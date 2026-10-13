#include <iostream>
using namespace std;

class WaitingRoom
{
private:
    int patients[100];
    int frontIndex;
    int count;
    int nextID;
public:
    WaitingRoom()
    {
        frontIndex = 0;
        count = 0;
        nextID = 1;
    }
    void RegisterPatient()
    {
        if (count == 100)
        {
            cout << "Waiting room is full." << endl;
            return;
        }
        int rear = (frontIndex + count) % 100;
        patients[rear] = nextID;
        cout << "Patient registered. ID: " << nextID << endl;

        nextID++;
        count++;
    }
    void ServePatient()
    {
        if (count == 0)
        {
            cout << "No patients waiting." << endl;
            return;
        }
        cout << "Serving patient ID: "
             << patients[frontIndex] << endl;

        frontIndex = (frontIndex + 1) % 100;
        count--;
    }
    void CancelAll()
    {
        count = 0;
        frontIndex = 0;
        cout << "All waiting appointments cancelled." << endl;
    }
    bool CanDoctorGoHome()
    {
        return count == 0;
    }
    void ShowAllPatient()
    {
        if (count == 0)
        {
            cout << "No patients waiting." << endl;
            return;
        }
        cout << "Waiting patient IDs: ";

        for (int i = 0; i < count; i++)
        {
            int index = (frontIndex + i) % 100;
            cout << patients[index] << " ";
        }
        cout << endl;
    }
};
int main()
{
    WaitingRoom room;
    int choice;
    do {
        cout << "\n--- Waiting Room ---" << endl;
        cout << "1. Register Patient" << endl;
        cout << "2. Serve Patient" << endl;
        cout << "3. Cancel All" << endl;
        cout << "4. Check Doctor Status" << endl;
        cout << "5. Show Waiting Patients" << endl;
        cout << "0. Exit" << endl;
        cout << "Enter choice: ";
        cin >> choice;

        if (choice == 1)
            room.RegisterPatient();
        else if (choice == 2)
            room.ServePatient();
        else if (choice == 3)
            room.CancelAll();
        else if (choice == 4)
        {
            if (room.CanDoctorGoHome())
                cout << "Doctor can go home." << endl;
            else
                cout << "Patients are still waiting." << endl;
        }
        else if (choice == 5)
            room.ShowAllPatient();
        else if (choice != 0)
            cout << "Invalid choice." << endl;

    } while (choice != 0);
    return 0;
}