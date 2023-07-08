#include <iostream>
#include <string>

using namespace std;

// Guest class
class Guest {
public:
    string name;
    int age;
    Guest(string name, int age) {
        this->name = name;
        this->age = age;
    }
};

// Room class
class Room {
    public:
        int room_number;
        string type;
        double price_per_night;
        bool is_available;
        string room_type;
        Guest* guest;
        Room* next;

        // Constructor
        Room(int room_number, string type, double price_per_night, string room_type)
        {
            this->room_number = room_number;
            this->type = type;
            this->price_per_night = price_per_night;
            this->room_type = room_type;
            is_available = true;
            guest = NULL;
            next = NULL;
        }
};

// Linked list implementation of the queue
class Queue {
    public:
        Room* front;
        Room* rear;
        int count;

        public:
        // Constructor
        Queue()
        {
            front = NULL;
            rear = NULL;
            count = 0;
        }

        // Add a room to the queue
        void enqueue(Room* room)
        {
            if (front == NULL)        //The queue is empty
                {
                    front = room;
                    rear = room;
                }
            else
                {
                    rear->next = room;
                    rear = room;
                }
            count++;   //size of the room will be increamented
        }

        // Remove a room from the queue
        Room* dequeue()
        {
            if (front == NULL)
                {
                    return NULL;
            }
            else
                {
                    Room* room = front;
                    front = front->next;
                    room->next = NULL;
                    count--;
                    return room;
                }
        }

        // Check if the queue is empty
        bool isEmpty()
        {
            return (front == NULL);
        }

        // Print the contents of the queue
        void print(bool isAvailable)
        {
            Room* current = front;
            while (current != NULL)
                {
                    if (current->is_available == isAvailable)
                    {
                        cout << "Room " << current->room_number << " (" << current->type << ")";
                        if (current->is_available)
                        {
                            cout << " - Available";
                        }
                        else
                            {
                                cout << " - Occupied by " << current->guest->name << " (Age: " << current->guest->age << ")";
                            }
                            cout << " - Price per night: $" << current->price_per_night << " - Room type: " << current->room_type << endl;
                       }
                       current = current->next;
                }
}

        // Returns a pointer to the front element of the queue
        Room* getFront() const
        {
            return front;
        }

};

// Binary search tree implementation for searching rooms
class BST{
    private:
        struct Node
        {
        Room* room;
        Node* left;
        Node* right;
        Node(Room* room)     //constructor
        {
            this->room = room;
            left = NULL;
            right = NULL;
        }
    };
    Node* root;

    public:

    // Constructor
    BST()
    {
        root = NULL;
    }

    // Search for a room by room number
    Room* search(int roomNumber)
    {
        Node* current = root;
        while (current != NULL)
        {
            if (roomNumber == current->room->room_number)
            {
                return current->room;
            }
            else if (roomNumber < current->room->room_number)
            {
                current = current->left;
            }
            else
            {
                current = current->right;
            }
        }
        return NULL;
    }
};

// Hotel class
class Hotel {
    private:
        Queue availableRooms;
        Queue occupiedRooms;
        int totalRooms;
        BST rooms;

        public:

        // Constructor
        Hotel(int totalRooms)
        {
            this->totalRooms = totalRooms;
            // Create Room objects and enqueue them onto availableRooms queue
            for (int i = 1; i <=totalRooms; i++)
            {
                availableRooms.enqueue(new Room(i, "Standard", 0.0, "Single OR Double"));
            }
        }
        // Check in a guest to a room
        void checkIn()
        {
            if (availableRooms.isEmpty())
            {
                cout << "Sorry, no rooms available." << endl;
                return;
            }
            string guestName;
            int guestAge;
            cout << "Enter guest name: ";
            cin >> guestName;
            cout << "Enter guest age: ";
            cin >> guestAge;
            string roomType;
            cout << "Enter room type (Single or Double): ";
            cin >> roomType;
            if (roomType != "Single" && roomType != "Double")
            {
                cout << "Invalid room type." << endl;
                return;
            }
            double pricePerNight;
            if (roomType == "Double")
            {
                pricePerNight = 150.0;
                cout << "Price per night for a double room is $" << pricePerNight << "." << endl;
            }
            else
                {
                    pricePerNight = 100.0;
                    cout << "Price per night for a single room is $" << pricePerNight << "." << endl;
                }
                Room* room = availableRooms.dequeue();
                room->is_available = false;
                room->guest = new Guest(guestName, guestAge);
                room->room_type = roomType;
                room->price_per_night = pricePerNight;
                occupiedRooms.enqueue(room);
                cout <<"Checked in " << guestName << " (Age: " << guestAge << ") to room " << room->room_number<< " (Type: " << roomType << ")" << endl;
                cout<<endl;
            }

    // Check out a guest from a room
    void checkOut()
    {
        if (occupiedRooms.isEmpty())
        {
            cout << "No guests to check out." << endl;
            return;
        }
        string guestName;
        cout << "Enter guest name: ";
        cin >> guestName;
        Room* room = NULL;
        Room* current = occupiedRooms.front;
        while (current != NULL)
        {
            if (current->guest->name == guestName)
            {
                room = current;
                break;
            }
            current = current->next;
        }
        if (room == NULL)
        {
            cout << "Guest not found." << endl;
            return;
        }
        room->is_available = true;
        Guest* guest = room->guest;
        room->guest = NULL;
        availableRooms.enqueue(room);
        occupiedRooms.dequeue();
        cout << "Guest " << guestName << " has been checkedout from room " << room->room_number << "." << endl;
        cout<<endl;
    }

    // Print the status of all rooms
    void printStatus()
    {
        cout << "Available rooms:" << endl;
        availableRooms.print(true);
        cout << "Occupied rooms:" << endl;
        occupiedRooms.print(false);
        cout<<endl;
    }

    // Search for a room by room number
    void searchRoom()
    {
        int roomNumber;
        cout << "Enter room number: ";
        cin >> roomNumber;

        // Search for the room in the BST
        Room* room = rooms.search(roomNumber);

        if (room == NULL)
            {
                // If room not found in BST, search in the availableRooms queue
                Room* current = availableRooms.front;
                while (current != NULL)
                {
                    if (current->room_number == roomNumber)
                    {
                        room = current;
                        //break;
                    }
                    current = current->next;
                }

        // If room not found in availableRooms queue, search in the occupiedRooms queue
        if (room == NULL)
        {
            current = occupiedRooms.front;
            while (current != NULL)
            {
                if (current->room_number == roomNumber)
                {
                    room = current;
                    break;
                }
                current = current->next;
            }
        }

        // If room still not found, print message
        if (room == NULL)
        {
            cout << "Room not found." << endl;
        }
    }

    // If room found in BST, print its details
    if (room != NULL)
    {
        cout << "Room " << room->room_number << " (" << room->type << ")";
        if (room->is_available)
        {
            cout << " - Available";
        }
        else
        {
            cout << " - Occupied by " << room->guest->name << " (Age: " << room->guest->age << ")";
        }
        cout << " - Price per night: $" << room->price_per_night<< " - Room type: " << room->room_type << endl;
        cout<<endl;
    }
}
};

// Main function
int main()
{
    // Create a hotel object with 10 rooms
    Hotel hotel(10);

    while (true) {
        // Display menu
        cout << "Hotel Management System" << endl;
        cout << "-------------------------" << endl;
        cout << "1. Check in" << endl;
        cout << "2. Check out" << endl;
        cout << "3. Print status" << endl;
        cout << "4. Search for a room" << endl;
        cout << "5.Exit" << endl;
        cout << "Enter your choice: ";

        // Get user input
        int choice;
        cin >> choice;

        switch (choice) {
        case 1:
            hotel.checkIn();
            break;
        case 2:
            hotel.checkOut();
            break;
        case 3:
            hotel.printStatus();
            break;
        case 4:
            hotel.searchRoom();
            break;
        case 5:
            cout << "Exiting program..." << endl;
            return 0;
        default:
            cout << "Invalid choice." << endl;
            break;
        }

    }
}
