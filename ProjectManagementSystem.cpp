#include <iostream>
#include <deque>
#include <forward_list>
#include <fstream>
#include <string>
#include <cstdlib> // system("cls")
#include <ctime>   // norint parodyti tikslu projekto sukurimo laika
#include <limits>  // del numeric_limits
#pragma warning(disable : 4996) // ignoruoja Visual Studio ispejima del localtime

using namespace std;

// sita funkcija valo input buffer kad nebutu nematomu tarpu del \n ir panasiai
void clearInputBuffer()
{
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

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
    int skill;
    deque<Task> tasks; // dekas
    TeamMember* next;  // TS pointeris
};

struct Project
{
    string title;
    time_t startTime;
    forward_list<TeamMember> teamMembers; // tiesinis sarasas
};

string GetCurrentDateTime()
{
    time_t currentTime = time(nullptr);      // gaunamas dabartinis laikas
    tm* timeInfo = localtime(&currentTime); // konvertuoja laika i struct tm
    char buffer[20];
    strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", timeInfo); // formatuoja data ir laika
    return string(buffer);
}

void DeleteTask(Project& project);
void UpdateReport(const Project& project, const string& changeDescription);
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

    while (true)
    {
        system("cls");

        cout << "\n>>> Project Management System <<<\n";
        cout << "1. Create Project\n";
        cout << "2. Create Team\n";
        cout << "3. Create & Assign Task\n";
        cout << "4. View Tasks\n";
        cout << "5. Generate Individual Task File\n";
        cout << "6. Reassign Task\n";
        cout << "7. Delete Task\n";
        cout << "8. Finish Project\n";
        cout << "9. Exit\n";
        cout << "Select an option: ";
        int menuChoice;
        cin >> menuChoice;

        switch (menuChoice)
        {
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
            ReassignTask(currentProject);
            break;
        case 7:
            DeleteTask(currentProject);
            break;
        case 8:
            FinishProject(currentProject);
            break;
        case 9:
            cout << "PROGRAM CLOSED.\n";
            return 0;
        default:
            cout << "Invalid option.\n";
            clearInputBuffer();
            cin.get();
        }
    }

    return 0;
}

void CreateProject(Project& project)
{
    project.startTime = time(nullptr); // issaugo project start time
    char buffer[20];
    strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", localtime(&project.startTime));

    cout << "Enter project title: ";
    clearInputBuffer();
    getline(cin, project.title);

    ofstream projectFile("project.txt");
    projectFile << "Project Title: " << project.title << "\nStart Date: " << buffer << "\n";
    projectFile.close();

    cout << "Project created successfully.\n";

    cout << "<<<Type any key to continue>>>\n";
    clearInputBuffer();
    cin.get();
}

void CreateTeam(Project& project)
{
    cout << "Enter the number of team members: ";
    int numMembers;
    cin >> numMembers;

    clearInputBuffer();

    for (int i = 0; i < numMembers; i++)
    {
        TeamMember newMember;

        cout << "Enter team member name: ";
        getline(cin, newMember.name);

        cout << "Enter team member position: ";
        getline(cin, newMember.position);

        cout << "Enter team member skill (1 - 6): ";
        cin >> newMember.skill;

        newMember.tasks.clear();

        project.teamMembers.push_front(newMember);

        clearInputBuffer();
    }

    cout << "Team created successfully.\n";
    cout << "<<<Type any key to continue>>>\n";
    clearInputBuffer();
    cin.get();
}

void CreateTask(Project& project)
{
    string memberName;
    cout << "Enter team member name: ";
    clearInputBuffer();
    getline(cin, memberName);

    auto it = project.teamMembers.begin();

    while (it != project.teamMembers.end())
    {
        if (it->name == memberName)
        {
            Task task;
            cout << "Enter task title: ";
            getline(cin, task.title);

            cout << "Enter task description: ";
            getline(cin, task.description);

            cout << "Enter task priority (3 - High, 2 - Medium, 1 - Low): ";
            cin >> task.priority;

            it->tasks.push_back(task);
            cout << "Task assigned successfully.\n";
            cout << "<<<Type any key to continue>>>\n";
            clearInputBuffer();
            cin.get();

            // UPDATE REPORT
            UpdateReport(project, "Task assigned to " + it->name + ".\n");
            return;
        }

        ++it;
    }

    cout << "Team member not found.\n";
    cout << "<<<Type any key to continue>>>\n";
    clearInputBuffer();
    cin.get();
}

void ViewTasks(const Project& project)
{
    auto it = project.teamMembers.begin();

    if (it == project.teamMembers.end())
    {
        cout << "No team members in the project.\n";
        cout << "<<<Type any key to continue>>>\n";
        cin.ignore();
        cin.get();
        return;
    }

    do
    {
        cout << "\nTeam Member: " << it->name << "\nPosition: " << it->position << "\nSkill Level: " << it->skill << "\nTasks:\n";

        if (it->tasks.empty())
        {
            cout << "No tasks assigned.\n";
        }
        else
        {
            for (const auto& task : it->tasks)
            {
                cout << "  Title: " << task.title << "\n  Description: " << task.description
                    << "\n  Priority (3 - High ; 2 - Medium ; 1 - Low): " << task.priority << "\n\n";
            }
        }
        ++it;
    } while (it != project.teamMembers.end());

    cout << "<<<Type any key to continue>>>\n";
    cin.ignore();
    cin.get();
}

void GenerateTaskFile(const Project& project)
{
    auto it = project.teamMembers.begin();

    if (it == project.teamMembers.end())
    {
        cout << "No team members in the project.\n";
        cout << "<<<Type any key to continue>>>\n";
        clearInputBuffer();
        cin.get();
        return;
    }

    do
    {
        ofstream taskFile(it->position + "_tasks.txt");

        taskFile << "Team Member: " << it->name << "\nPosition: " << it->position << "\n\nTasks:\n";

        if (it->tasks.empty())
        {
            taskFile << "No tasks assigned.\n";
        }
        else
        {
            for (const auto& task : it->tasks)
            {
                taskFile << "  Title: " << task.title << "\n  Description: " << task.description
                    << "\n  Priority: " << task.priority << "\n\n";
            }
        }

        // susumuoja visu task priority skaicius
        int totalPriority = 0;
        for (const auto& task : it->tasks)
        {
            totalPriority += task.priority;
        }
        // zemiau tikriname ar spes ar ne
        string completionStatus = (it->skill >= totalPriority) ? "On Time" : "Late";
        taskFile << "Estimated to complete: " << completionStatus << "\n";
        taskFile.close();

        cout << "Task file generated for " << it->name << ".\n";
        cout << "<<<Type any key to continue>>>\n";
        clearInputBuffer();
        cin.get();

        ++it;
    } while (it != project.teamMembers.end());
}

void UpdateReport(const Project& project, const string& changeDescription)
{
    ofstream reportFile("report.txt", ios::app); // pridedame (append) teksta prie egzistuojancio report.txt
    reportFile << "Change date: " << GetCurrentDateTime() << "\n";

    auto it = project.teamMembers.begin();
    while (it != project.teamMembers.end())
    {
        reportFile << it->name << ", " << it->position << "\n";
        for (const auto& task : it->tasks)
        {
            reportFile << "  Title: " << task.title << "\n  Description: " << task.description
                << "\n  Priority: " << task.priority << "\n\n";
        }

        ++it;
    }

    reportFile << changeDescription << "\n\n";
    reportFile.close();
}

void ReassignTask(Project& project)
{
    string sourceMemberName, destinationMemberName;

    cout << "Enter the name of the team member from whose tasks you want to reassign: ";
    clearInputBuffer();
    getline(cin, sourceMemberName);

    auto sourceMember = project.teamMembers.begin();

    while (sourceMember != project.teamMembers.end())
    {
        if (sourceMember->name == sourceMemberName)
        {
            break; // radome pradini member
        }
        ++sourceMember;

        if (sourceMember == project.teamMembers.end()) // jei perejom per visus ir neradom
        {
            cout << "Team member not found.\n";
            cout << "<<<Type any key to continue>>>\n";
            clearInputBuffer();
            cin.get();
            return;
        }
    }

    // tikriname ar sourceMember egzistuoja
    if (sourceMember == project.teamMembers.end())
    {
        cout << "Team member not found.\n";
        cout << "<<<Type any key to continue>>>\n";
        clearInputBuffer();
        cin.get();
        return;
    }

    cout << "\nTasks of " << sourceMemberName << ":\n";
    int taskCount = 1;
    for (const auto& task : sourceMember->tasks)
    {
        cout << taskCount << ". Title: " << task.title << "\n   Description: " << task.description
            << "\n   Priority: " << task.priority << "\n\n";
        taskCount++;
    }

    if (sourceMember->tasks.empty())
    {
        cout << "No tasks to reassign.\n";
        cout << "<<<Type any key to continue>>>\n";
        clearInputBuffer();
        cin.get();
        return;
    }

    int selectedTask;
    cout << "Enter the number of the task you want to reassign: ";
    cin >> selectedTask;

    if (selectedTask < 1 || selectedTask > sourceMember->tasks.size())
    {
        cout << "Invalid task number.\n";
        cout << "<<<Type any key to continue>>>\n";
        clearInputBuffer();
        cin.get();
        return;
    }

    Task reassignTask;
    auto it = sourceMember->tasks.begin();
    advance(it, selectedTask - 1);
    reassignTask = *it;
    sourceMember->tasks.erase(it);

    cout << "Enter the name of the team member to whom you want to reassign the task: ";
    clearInputBuffer();
    getline(cin, destinationMemberName);

    auto destinationMember = project.teamMembers.begin();

    while (destinationMember != project.teamMembers.end())
    {
        if (destinationMember->name == destinationMemberName)
        {
            destinationMember->tasks.push_back(reassignTask);
            cout << "Task reassigned successfully.\n";
            cout << "<<<Type any key to continue>>>\n";
            clearInputBuffer();
            cin.get();

            UpdateReport(project, "Task reassigned from " + sourceMemberName + " to " + destinationMemberName + ".\n");

            return;
        }
        ++destinationMember;

        if (destinationMember == project.teamMembers.end())
        {
            cout << "Destination team member not found.\n";
            cout << "<<<Type any key to continue>>>\n";
            clearInputBuffer();
            cin.get();
            return;
        }
    }
}

void DeleteTask(Project& project)
{
    string memberName;
    cout << "Enter team member name: ";
    clearInputBuffer();
    getline(cin, memberName);

    auto it = project.teamMembers.begin();

    while (it != project.teamMembers.end())
    {
        if (it->name == memberName)
        {
            if (it->tasks.empty())
            {
                cout << "No tasks to delete.\n";
                cout << "<<<Type any key to continue>>>\n";
                clearInputBuffer();
                cin.get();
                return;
            }

            cout << "\nTasks of " << memberName << ":\n";
            int taskCount = 1;
            for (const auto& task : it->tasks)
            {
                cout << taskCount << ". Title: " << task.title << "\n   Description: " << task.description
                    << "\n   Priority: " << task.priority << "\n\n";
                taskCount++;
            }

            int selectedTask;
            cout << "Enter the number of the task you want to delete: ";
            cin >> selectedTask;

            if (selectedTask < 1 || selectedTask > it->tasks.size())
            {
                cout << "Invalid task number.\n";
                cout << "<<<Type any key to continue>>>\n";
                clearInputBuffer();
                cin.get();
                return;
            }

            auto taskIt = it->tasks.begin();
            advance(taskIt, selectedTask - 1);
            it->tasks.erase(taskIt);

            cout << "Task deleted successfully.\n";
            cout << "<<<Type any key to continue>>>\n";
            clearInputBuffer();
            cin.get();

            UpdateReport(project, "Task deleted from " + memberName + "'s list.\n");

            return;
        }

        ++it;
    }

    cout << "Team member not found.\n";
    cout << "<<<Type any key to continue>>>\n";
    clearInputBuffer();
    cin.get();
}


void FinishProject(Project& project)
{
    // Tikriname ar isvis egzistuoja project.txt
    fstream checkFile("project.txt", ios::in);

    if (!checkFile.is_open())
    {
        cout << "ERROR: project.txt does not exist. Please create a project first.\n";
        cout << "<<<Type any key to continue>>>\n";
        clearInputBuffer();
        cin.get();
        return;
    }
    checkFile.close();

    // ios::app (append) atidaro egzistuojanti project.txt
    ofstream projectFile("project.txt", ios::app);

    // suraso info
    projectFile << "\nEnd Date: " << GetCurrentDateTime() << "\nStatus: ";

    auto it = project.teamMembers.begin();
    bool finishedOnTime = true;

    while (it != project.teamMembers.end())
    {
        int totalPriority = 0;
        for (const auto& task : it->tasks)
        {
            totalPriority += task.priority;
        }

        if (it->skill < totalPriority) // tikrina ar project baigiasi laiku
        {
            finishedOnTime = false;
            break;
        }

        ++it;
    }

    projectFile << (finishedOnTime ? "FINISHED ON TIME" : "FINISHED LATE") << "\n";

    projectFile.close();

    cout << "Project finish report added to project.txt.\n";
    cout << "<<<Type any key to continue>>>\n";
    clearInputBuffer();
    cin.get();
}
