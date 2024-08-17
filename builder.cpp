//Builder pattern 

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

class MazeBuilder {
public:
    virtual void BuildMaze() {}
    virtual void BuildRoom(int room) {}
    virtual void BuildDoor(int roomFrom, int roomTo) {}
    virtual Maze* GetMaze() {return 0; }
protected:
    MazeBuilder() {};
};

class MazeGame
{
public:
    Maze* CreateMaze();
    Maze* CreateMaze(MazeBuilder& builder);
    Maze* CreateComplexMaze(MazeBuilder& builder);
};

Maze* MazeGame::CreateMaze(MazeBuilder& builder)
{
    builder.BuildMaze();
    builder.BuildRoom(1);
    builder.BuildRoom(2);
    builder.BuildDoor(1, 2);
    return builder.GetMaze();
}

Maze* MazeGame::CreateComplexMaze(MazeBuilder& builder)
{
    builder.BuildRoom(1);
    //...
    builder.BuildRoom(1001);

    return builder.GetMaze();
}

class StandardMazeBuilder: public MazeBuilder
{
public:
    StandardMazeBuilder();
    virtual void BuildMaze();
    virtual void BuildRoom(int);
    virtual void BuildDoor(int, int);
    virtual Maze* GetMaze();
private:
    Direction CommonWall(Room* r1, Room* r2)
    {
        if (typeid(r1->GetSide(North)) == typeid(Door))
        {
            return North;
        }
        else if (typeid(r2->GetSide(East)) == typeid(Door))
        {
            return East;
        }   
        else if (typeid(r1->GetSide(West)) == typeid(Door))
        {
            return West;
        }
        else
        {
            return South;
        }
        
    }
    Maze* m_currentMaze;
};

StandardMazeBuilder::StandardMazeBuilder()
{
    m_currentMaze = 0;
}

void StandardMazeBuilder::BuildMaze() 
{
    m_currentMaze = new Maze;
}

Maze* StandardMazeBuilder::GetMaze() 
{
    return m_currentMaze;
}

void StandardMazeBuilder::BuildRoom(int n) 
{
    if(!m_currentMaze->RoomNo(n))
    {
        Room* room = new Room(n);
        m_currentMaze->addRoom(room);

        room->SetSide(North, new Wall);
        room->SetSide(South, new Wall);
        room->SetSide(East, new Wall);
        room->SetSide(West, new Wall);
    }
}

void StandardMazeBuilder::BuildDoor(int n1, int n2)
{
    Room* r1 = m_currentMaze->RoomNo(n1);
    Room* r2 = m_currentMaze->RoomNo(n2);
    Door* d = new Door(r1, r2);
    r1->SetSide(CommonWall(r1,r2), d);
    r2->SetSide(CommonWall(r2,r1), d);
}

void test1()
{
    Maze* maze;
    MazeGame game;
    StandardMazeBuilder builder;
    maze = builder.GetMaze();
}

class CountingMazeBuilder: public MazeBuilder
{
public:
    CountingMazeBuilder();
    virtual void BuildMaze() {}
    virtual void BuildRoom(int);
    virtual void BuildDoor(int, int);
    virtual void AddWall(int, Direction) {}
    void GetCounts(int&, int&);
private:
    int m_doors;
    int m_rooms;
};

CountingMazeBuilder::CountingMazeBuilder()
{
    m_rooms = m_doors = 0;
}

void CountingMazeBuilder::BuildRoom(int)
{
    m_rooms++;
}

void CountingMazeBuilder::BuildDoor(int, int)
{
    m_doors++;
}

void CountingMazeBuilder::GetCounts(int& rooms, int& doors)
{
    rooms = m_rooms;
    doors = m_doors;
}

void test2()
{
    int rooms, doors;
    MazeGame game;
    CountingMazeBuilder builder;
    game.CreateMaze(builder);
    builder.GetCounts(rooms, doors);
    std::cout << "Labyrinth contains: " << rooms << " rooms and " << doors << " doors" << std::endl;
}
int main()
{
    test1();
    test2();
	return 0;
}