
//Abstract Factory pattern
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
private:
    MapSite* m_sides[4];
    int m_roomNumber;
};

class Wall: public MapSite
{
public:
    Wall() {}
    virtual void Enter() {}
};

class Door: public MapSite
{
public:
    Door(Room* r1 = 0, Room* r2 = 0): m_room1(r1), m_room2(r2) {}
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

class Maze
{
public:
    Maze(){}
    void addRoom(Room* newRoom)
    {
        m_rooms[newRoom->getRoomNumber()] = newRoom;
    }
    Room* RoomNo(int n) const
    {
        return m_rooms.at(n);
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
        return new Door(r1, r2);
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
    Door *theDoor = new Door(r1, r2);

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
//Enchanted variant room
class Spell
{
public:
    Spell() {}
};

class EnchantedRoom: public Room
{
public:
    EnchantedRoom(int n, Spell* CastSpell): Room(n)
    {
        m_spell = CastSpell;
    }
protected:
    Spell* m_spell;
};

class EnchantedMazeFactory: public MazeFactory
{
public:
    EnchantedMazeFactory() {}
    virtual Room* MakeRoom(int n) const
    {
        return new EnchantedRoom(n, CastSpell());
    }
protected:
    Spell* CastSpell() const
    {
        return new Spell;
    }
};

// Bomb in room:
class RoomWithBomb: public Room
{
public:
    RoomWithBomb(int n): Room(n) {}
    bool isBombInRoom()
    {
        return isBomb;
    }
    bool isBombDetonated()
    {
        return isDetonated;
    }
private:
    bool isBomb;
    bool isDetonated;
};

class BombedWall: public Wall
{
public:
    BombedWall() {}
    bool WallState()
    {
        return isDestroyed;
    }
private:
    bool isDestroyed;
};

class BombedMazeFactory: public MazeFactory
{
    virtual Wall* MakeWall() const
    {
        return new BombedWall;
    }
    virtual Room* MakeRoom(int n) const
    {
        return new RoomWithBomb(n);
    }
};

void test()
{
    MazeGame game;
    BombedMazeFactory factory;
    game.CreateMaze(factory);
}
int main()
{
    test();
}