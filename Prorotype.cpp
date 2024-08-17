//Prototype pattern
#include <iostream>
#include <unordered_map>

enum Direction{North, South, East, West};

//Base class
class MapSite
{
public:
    virtual void Enter() = 0;
};


class Room: public MapSite
{
public:
    Room() = default;
    Room(int roomNo)
    {
        m_roomNumber = roomNo;
    }
    MapSite* GetSide(Direction side) const
    {
        return m_sides[side];
    }
    void SetSide(Direction side, MapSite* type)
    {
        m_sides[side] = type;
    }
    virtual void Enter() {}
    int getRoomNumber()
    {
        return m_roomNumber;
    }
    Room* Clone() const
    {
        return new Room(*this);
    }
private:
    MapSite* m_sides[4];
    int m_roomNumber;
};

class Wall: public MapSite
{
public:
    Wall() = default;
    Wall(const Wall&) = default;
    Wall* Clone() const;
    virtual void Enter() {}
};

Wall* Wall::Clone() const
{
    return new Wall(*this);
}

class Door: public MapSite
{
public:
    Door() = default;
    Door(const Door&);
    virtual void initialize(Room*, Room*);
    virtual Door* Clone() const;

    virtual void Enter(){}
    Room* OtherSideFrom(Room* from)
    {
        if (from == m_room1)
        {
            return m_room2;
        }
        else
        {
            return m_room1;
        }
    }
private:
    Room* m_room1;
    Room* m_room2;
    bool m_IsOpen;
};

Door::Door(const Door& other)
{
    m_room1 = other.m_room1;
    m_room2 = other.m_room2;
    m_IsOpen = other.m_IsOpen;
}

void Door::initialize(Room* r1, Room* r2)
{
    m_room1 = r1;
    m_room2 = r2;
}

Door* Door::Clone() const
{
    return new Door(*this);
}

class Maze
{
public:
    Maze() = default;
    Maze(const Maze&) = default;
    void addRoom(Room* newRoom)
    {
        m_rooms[newRoom->getRoomNumber()] = newRoom;
    }
    Room* RoomNo(int n) const
    {
        return m_rooms.at(n);
    }
    Maze* Clone() const
    {
        return new Maze(*this);
    }
private:
    std::unordered_map<int, Room*> m_rooms;
};



class MazeFactory
{
public:
    MazeFactory(){}

    virtual Maze* MakeMaze() const
    {
        return new Maze;
    }
    virtual Wall* MakeWall() const
    {
        return new Wall;
    }
    virtual Room* MakeRoom(int n) const
    {
        return new Room(n);
    }
    virtual Door* MakeDoor(Room* r1, Room* r2) const
    {
        return new Door;
    }
};

class MazeGame
{
public:
    Maze* CreateMaze();
    Maze* CreateMaze(MazeFactory& factory);
};

Maze* MazeGame::CreateMaze()
{
    Maze *aMaze = new Maze;
    Room *r1 = new Room(1);
    Room *r2 = new Room(2);
    Door *theDoor = new Door;

    aMaze->addRoom(r1);
    aMaze->addRoom(r2);

    r1->SetSide(North, new Wall);
    r1->SetSide(East, theDoor);
    r1->SetSide(West, new Wall);
    r1->SetSide(South, new Wall);

    r2->SetSide(North, new Wall);
    r2->SetSide(East, new Wall);
    r2->SetSide(South, new Wall);
    r2->SetSide(West, theDoor);

    return aMaze;
}

Maze* MazeGame::CreateMaze(MazeFactory& factory)
{
    Maze* aMaze = factory.MakeMaze();
    Room* r1 = factory.MakeRoom(1);
    Room* r2 = factory.MakeRoom(2);
    Door* theDoor = factory.MakeDoor(r1, r2);

    aMaze->addRoom(r1);
    aMaze->addRoom(r2);

    r1->SetSide(North, factory.MakeWall());
    r1->SetSide(East, theDoor);
    r1->SetSide(West, factory.MakeWall());
    r1->SetSide(South, factory.MakeWall());

    r2->SetSide(North, factory.MakeWall());
    r2->SetSide(East, factory.MakeWall());
    r2->SetSide(South, factory.MakeWall());
    r2->SetSide(West, theDoor);
    return aMaze;
}

class MazeProtypeFactory: public MazeFactory
{
public:
    MazeProtypeFactory(Maze*, Wall*, Room*, Door*);
    virtual Maze* MakeMaze() const;
    virtual Room* MakeRoom() const;
    virtual Wall* MakeWall() const;
    virtual Door* MakeDoor(Room*, Room*) const;
private:
    Maze* m_MazePrototype;
    Room* m_RoomPrototype;
    Wall* m_WallPrototype;
    Door* m_DoorPrototype;
};

MazeProtypeFactory::MazeProtypeFactory(Maze* m, Wall* w, Room* r, Door* d)
{
    m_DoorPrototype = d;
    m_MazePrototype = m;
    m_RoomPrototype = r;
    m_WallPrototype = w;
}

Wall* MazeProtypeFactory::MakeWall() const
{
    return m_WallPrototype->Clone();
}

Door* MazeProtypeFactory::MakeDoor(Room* r1, Room* r2) const
{
    Door* door = m_DoorPrototype->Clone();
    door->initialize(r1, r2);
    return door;
}

Room* MazeProtypeFactory::MakeRoom() const
{
    Room* room = m_RoomPrototype->Clone();
    return room;
}

Maze* MazeProtypeFactory::MakeMaze() const
{
    Maze* maze = m_MazePrototype->Clone();
    return maze;
}

void test()
{
    MazeGame game;
    MazeProtypeFactory simpleMazeFactory(
        new Maze, new Wall, new Room, new Door);
    Maze* maze = game.CreateMaze(simpleMazeFactory);
}

class BombedWall: public Wall
{
public:
    BombedWall() = default;
    BombedWall(const BombedWall&);
    virtual Wall* Clone() const;
    bool HasBomb();
private:
    bool m_bomb;
};

BombedWall::BombedWall(const BombedWall& other): Wall(other)
{
    m_bomb = other.m_bomb;
}

Wall* BombedWall::Clone() const
{
    return new BombedWall(*this);
}

void test2()
{
    MazeGame game;
    MazeProtypeFactory simpleMazeFactory(
        new Maze, new BombedWall, new Room, new Door);
    Maze* maze = game.CreateMaze(simpleMazeFactory);
}

int main()
{
    test();
    test2();
    return 0;
}