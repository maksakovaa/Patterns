//Factory Method

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

class MazeGame
{
public:
    Maze* CreateMaze();

    //Fabrique methods:

    virtual Maze* MakeMaze() const
    {
        return new Maze;
    }
    virtual Room* MakeRoom(int n) const
    {
        return new Room(n);
    }
    virtual Wall* MakeWall() const
    {
        return new Wall;
    }
    virtual Door* MakeDoor(Room* r1, Room* r2) const
    {
        return new Door(r1, r2);
    }
};

Maze* MazeGame::CreateMaze()
{
    Maze* aMaze = MakeMaze();
    Room* r1 = MakeRoom(1);
    Room* r2 = MakeRoom(2);
    Door* TheDoor = MakeDoor(r1, r2);
    aMaze->addRoom(r1);
    aMaze->addRoom(r2);

    r1->SetSide(North, MakeWall());
    r1->SetSide(East, TheDoor);
    r1->SetSide(West, MakeWall());
    r1->SetSide(South, MakeWall());

    r2->SetSide(North, MakeWall());
    r2->SetSide(East, MakeWall());
    r2->SetSide(South, MakeWall());
    r2->SetSide(West, TheDoor);

    return aMaze;
}

// Bomb in room:
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

class BombedMazeGame: public MazeGame
{
public:
    BombedMazeGame() {}
    virtual Wall* MakeWall() const
    {
        return new BombedWall;
    }
    virtual Room* MakeRoom(int n) const
    {
        return new RoomWithBomb(n);
    }
};

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

class DoorNeedingSpell: public Door
{
public:
    DoorNeedingSpell(Room* r1, Room* r2): Door(r1, r2) {}
    const std::string isNeedSpell()
    {
        return "need spell";
    }
};

class EnchantedMazeGame : public MazeGame
{
public:
    EnchantedMazeGame() {}
    virtual Room* MakeRoom(int n) const
    {
        return new EnchantedRoom(n, CastSpell());
    }
    virtual Door* MakeDoor(Room* r1, Room* r2)
    {
        return new DoorNeedingSpell(r1, r2);
    }
protected:
    Spell* CastSpell() const
    {
        return new Spell;
    }
};

void test1()
{
    MazeGame game;
    Maze* maze = game.CreateMaze();
    std::cout << "Room number check: " << (bool) (1 == maze->RoomNo(1)->getRoomNumber()) << std::endl; 
}
void test2()
{
    BombedMazeGame game;
    Maze* maze = game.CreateMaze();
    std::cout << "Bomb in room:" <<  ((RoomWithBomb*)maze->RoomNo(1))->isBombInRoom() << std::endl;
}
void test3()
{
    EnchantedMazeGame game;
    Maze* maze = game.CreateMaze();
    std::cout << ((DoorNeedingSpell*)maze->RoomNo(1)->GetSide(East))->isNeedSpell() <<std::endl;
}
int main()
{
    test1();
    test2();
    test3();
	return 0;
}