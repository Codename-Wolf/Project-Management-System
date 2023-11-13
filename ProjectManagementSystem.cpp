#include <iostream>
#include <deque>
#include <forward_list>
#include <fstream>
#include <string>
#include <cstdlib> //system("cls")
#include <ctime> //norint parodyti tikslu projekto sukurimo laika
#pragma warning(disable : 4996) //ignoruoja Visual Studio ispejima del localtime

/*
TODO:
1. In console not seeing error msges like member not found etc it just goes right to menu after clearing screen
2. Functions dont work in general / are unfinished
*/

using namespace std;

struct Task
{
    string title;
    string description;
    int priority;
};


struct TeamMember
{
    string name;
    string position;
    deque<Task> tasks;
    TeamMember* next; //TS pointeris
};

struct Project
{
    string title;
    TeamMember* teamMembers; //pointeris kuris yra TS galva
};

void CreateProject(Project& project);
void CreateTeam(Project& project);
void CreateTask(Project& project);
void ViewTasks(const Project& project);
void GenerateTaskFile(const Project& project);
void ReceiveReport(const Project& project);
void ReassignTask(Project& project);
void FinishProject(Project& project);

int main()
{
    Project currentProject;
    currentProject.teamMembers = nullptr;

	while (true)
	{
        system("cls");

        cout << "\nProject Management System\n";
        cout << "1. Create Project\n";
        cout << "2. Create Team\n";
        cout << "3. Create & Assign Task\n";
        cout << "4. View Tasks\n";
        cout << "5. Generate Individual Task File\n";
        cout << "6. Receive Individual Reports\n";
        cout << "7. Reassign Task\n";
        cout << "8. Finish Project\n";
        cout << "9. Exit\n";
        cout << "Select an option: ";
        int menuChoice;
        cin >> menuChoice;

        switch (menuChoice) {
        case 1:
            CreateProject(currentProject);
            break;
        case 2: 
            CreateTeam(currentProject);
            break;
        case 3:
            CreateTask(currentProject);
            break;
        case 4:
            ViewTasks(currentProject);
            break;
        case 5:
            GenerateTaskFile(currentProject);
            break;
        case 6:
            ReceiveReport(currentProject);
            break;
        case 7:
            ReassignTask(currentProject);
            break;
        case 8:
            FinishProject(currentProject);
            break;
        case 9:
            cout << "PROGRAM CLOSED.\n";
            return 0;
        default:
            cout << "Invalid option.\n";
        }

	}

    return 0;
}

void CreateProject(Project& project)
{
    time_t currentTime = time(nullptr); //gaunamas dabartinis laikas
    tm* timeInfo = localtime(&currentTime); //konvertuoja laika i struct tm
    char buffer[20];
    strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", timeInfo); //formatuoja data ir laika

    cout << "Enter project title: ";
    cin.ignore();
    getline(cin, project.title);

    ofstream projectFile("project.txt");
    projectFile << "Project Title: " << project.title << "\nStart Date: " << buffer << "\n";
    projectFile.close();

    cout << "Project created successfully.\n";

    cout << "<<<Type any key to continue>>>\n";
    cin.get();
}

void CreateTeam(Project& project)
{
    cout << "Enter the number of team members: ";
    int numMembers;
    cin >> numMembers;

    for (int i = 0; i < numMembers; i++)
    {
        TeamMember newMember;
        cout << "Enter team member name: ";
        cin.ignore();
        getline(cin, newMember.name);

        cout << "Enter team member position: ";
        cin.ignore();
        getline(cin, newMember.position);

        newMember.next = nullptr;  // Set the next pointer to nullptr for the new member

        if (project.teamMembers != nullptr) {
            // Find the last member in the team
            TeamMember* lastMember = project.teamMembers;
            while (lastMember->next != nullptr) {
                lastMember = lastMember->next;
            }
            // Set the next pointer of the last member to the new member
            lastMember->next = new TeamMember(newMember);
        }
        else {
            // If the team is empty, set the new member as the head of the team
            project.teamMembers = new TeamMember(newMember);
        }
    }
    cout << "Team created successfully.\n";
    cout << "<<<Type any key to continue>>>\n";
    cin.get();
}

void CreateTask(Project& project)
{
    string memberName;
    cout << "Enter team member name: ";
    cin.ignore();
    getline(cin, memberName);

    TeamMember* currentMember = project.teamMembers;

    while (currentMember != nullptr)
    {
        if (currentMember->name == memberName)
        {
            Task task;
            cout << "Enter task title: ";
            getline(cin, task.title);

            cout << "Enter task description: ";
            getline(cin, task.description);

            cout << "Enter task priority (1 - High, 2 - Medium, 3 - Low): ";
            cin >> task.priority;

            currentMember->tasks.push_back(task);
            cout << "Task assigned successfully.\n";
            cout << "<<<Type any key to continue>>>\n";
            cin.get();
            return;
        }

        currentMember = currentMember->next;

        if (currentMember == project.teamMembers)
        {
            cout << "Team member not found.\n";
            cout << "<<<Type any key to continue>>>\n";
            cin.get();
            return;
        }
    }
}

void ViewTasks(const Project& project)
{
    TeamMember* currentMember = project.teamMembers;

    if (currentMember == nullptr)
    {
        cout << "No team members in the project.\n";
        cout << "<<<Type any key to continue>>>\n";
        cin.get();
        return;
    }

    do
    {
        cout << "\nTeam Member: " << currentMember->name << "\nPosition: " << currentMember->position << "\nTasks:\n";

        for (const auto& task : currentMember->tasks) {
            cout << "  Title: " << task.title << "\n  Description: " << task.description
                 << "\n  Priority: " << task.priority << "\n\n";
        }
        currentMember = currentMember->next;
    } while (currentMember != nullptr && currentMember != project.teamMembers); //ciklas eina kol toliau yra sekantis team member, 
} //prie kurio dar nebuvo prieita (jei prieinamas priekinis "head", reiskias perejom visus team members).

void GenerateTaskFile(const Project& project) {
    TeamMember* currentMember = project.teamMembers;

    if (currentMember == nullptr) {
        cout << "No team members in the project.\n";
        cout << "<<<Type any key to continue>>>\n";
        cin.get();
        return;
    }

    do {
        ofstream taskFile(currentMember->name + "_tasks.txt");

        taskFile << "Team Member: " << currentMember->name << "\nPosition: " << currentMember->position << "\n\nTasks:\n";

        for (const auto& task : currentMember->tasks) {
            taskFile << "  Title: " << task.title << "\n  Description: " << task.description
                << "\n  Priority: " << task.priority << "\n\n";
        }

        taskFile.close();
        cout << "Task file generated for " << currentMember->name << ".\n";

        currentMember = currentMember->next;
    } while (currentMember != project.teamMembers);
}

void ReceiveReport(const Project& project) {
    TeamMember* currentMember = project.teamMembers;

    if (currentMember == nullptr) {
        cout << "No team members in the project.\n";
        cout << "<<<Type any key to continue>>>\n";
        cin.get();
        return;
    }

    do {
        // Assuming you have some way to determine the status of tasks for each member
        // You can modify this part based on the specifics of your program
        cout << "Report for " << currentMember->name << ":\n";
        for (const auto& task : currentMember->tasks) {
            cout << "  Title: " << task.title << "\n  Status: " << "StatusHere" << "\n\n";
        }

        currentMember = currentMember->next;
    } while (currentMember != project.teamMembers);
}

void ReassignTask(Project& project) {
    string sourceMemberName, destinationMemberName;

    cout << "Enter the name of the team member from whose tasks you want to reassign: ";
    cin.ignore();
    getline(cin, sourceMemberName);

    TeamMember* sourceMember = project.teamMembers;

    while (sourceMember != nullptr) {
        if (sourceMember->name == sourceMemberName) {
            break; // Found the source member
        }
        sourceMember = sourceMember->next;

        if (sourceMember == project.teamMembers) {
            cout << "Team member not found.\n";
            cout << "<<<Type any key to continue>>>\n";
            cin.get();
            return;
        }
    }

    // Check if sourceMember is NULL before entering the loop
    if (sourceMember == nullptr) {
        cout << "Team member not found.\n";
        cout << "<<<Type any key to continue>>>\n";
        cin.get();
        return;
    }

    cout << "\nTasks of " << sourceMemberName << ":\n";
    int taskCount = 1;
    for (const auto& task : sourceMember->tasks) {
        cout << taskCount << ". Title: " << task.title << "\n   Description: " << task.description
            << "\n   Priority: " << task.priority << "\n\n";
        taskCount++;
    }

    if (sourceMember->tasks.empty()) {
        cout << "No tasks to reassign.\n";
        cout << "<<<Type any key to continue>>>\n";
        cin.get();
        return;
    }

    int selectedTask;
    cout << "Enter the number of the task you want to reassign: ";
    cin >> selectedTask;

    if (selectedTask < 1 || selectedTask > sourceMember->tasks.size()) {
        cout << "Invalid task number.\n";
        cout << "<<<Type any key to continue>>>\n";
        cin.get();
        return;
    }

    Task reassignTask;
    auto it = sourceMember->tasks.begin();
    advance(it, selectedTask - 1);
    reassignTask = *it;
    sourceMember->tasks.erase(it);

    cout << "Enter the name of the team member to whom you want to reassign the task: ";
    cin.ignore();
    getline(cin, destinationMemberName);

    TeamMember* destinationMember = project.teamMembers;

    while (destinationMember != nullptr) {
        if (destinationMember->name == destinationMemberName) {
            destinationMember->tasks.push_back(reassignTask);
            cout << "Task reassigned successfully.\n";
            cout << "<<<Type any key to continue>>>\n";
            cin.get();
            return;
        }
        destinationMember = destinationMember->next;

        if (destinationMember == project.teamMembers) {
            cout << "Destination team member not found.\n";
            cout << "<<<Type any key to continue>>>\n";
            cin.get();
            return;
        }
    }
}

void FinishProject(Project& project) {
    // Add code here to finish the project and update the project document.
}