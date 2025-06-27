#include <iostream>
#include <string>
#include <vector>
#include <chrono>
#include <ctime>
#include <algorithm>
#include "../include/Task/Task.h"
#include "../include/cpp_json/cpp_json.h"
#include "../include/EXP_HTML/EXP_HTML.h"



 
    
    std::string Get_Today_Data(){
        auto now = std::chrono::system_clock::now();
        std::time_t time_now = std::chrono::system_clock::to_time_t(now);
        std::tm * ltm = std::localtime(&time_now);
        char buf[11];
        std::strftime(buf,sizeof(buf),"%Y-%m-%d",ltm);
        return std::string(buf);
    }



int main(){
    
    std::vector<Task> Tasks ; 
    loadTasks(Tasks);
    

    int choice;
    do{
        std::cout << "\n_____TASK MANAGER_____\n";
        std::cout << "1. Show Tasks\n";
        std::cout << "2. Add New Task\n";
        std::cout << "3. Mark Done Task\n";
        std::cout << "4. Delete Task\n";
        std::cout << "5. Show Task by category\n";
        std::cout << "6. Show overdue Tasks\n";
        std::cout << "7. Edit a task\n";
        std::cout << "8. Export Tasks to HTML\n";
        std::cout << "9. Sort tasks by deadline (soonest first)\n";
        std::cout << "10. Sort tasks by status (Not done first)\n";
        std::cout << "11. Search tasks by keyword\n";
        std::cout << "12. Show task statistics\n";
        std::cout << "13. Exit\n";      
        std::cout << "______________________\n";
        std::cout << "Select an option:";
        std::cin >> choice;
        std::cin.ignore();

        if(choice == 1){
            if(Tasks.empty()){
                std::cout << "📭 No Tasks found.\n";
            }else{
                std::cout << "\n_________Tasks________\n";
                for(size_t i = 0 ; i < Tasks.size();++i){
                   Tasks[i].show(i);
                }
            }
        }else if(choice == 2){
            std::string title , desc , cate , dl;
            std::cout << "Task name? ";
            std::getline(std::cin,title);
            std::cout << "Task description? ";
            std::getline(std::cin,desc);
            std::cout << "Task category? ";
            std::getline(std::cin,cate);
            std::cout << "Task deadline? (YYYY-MM-DD)";
            std::getline(std::cin,dl);
            Tasks.emplace_back(title,desc,cate,dl);
            std::cout << "\nTask added! ✅\n";
        }else if(choice == 3){
            int index;
            std::cout << "task number to mark as done: ";
            std::cin >> index;
            std::cin.ignore();

            if(index >= 0 && index < (int)Tasks.size()){
                Tasks[index].markDone();
                std::cout << "🎉 Task marked as done!\n";
            }else{
                std::cout << "❌ Invalid task number.\n";
            }
        }else if(choice == 4){
            int del_index;
            std::cout << "Enter task number to delete: ";
            std::cin >> del_index;
            std::cin.ignore();
            if(del_index >= 0 && del_index < (int)Tasks.size()){
                Tasks.erase(Tasks.begin() + del_index);
                std::cout << "🗑️ Task deleted!\n";
            } else {
                std::cout << "❌ Invalid index.\n";
            }

        }else if(choice == 5){
            std::string cat;
            std::cout << "Enter name of category filter: ";
            std::getline(std::cin,cat);
            bool found = false;

            for(size_t i = 0 ; i < Tasks.size();i++){
                if(Tasks[i].get_Category() == cat ){
                    Tasks[i].show(i);
                }
            }
            if(!found){
                std::cout <<  "📭 No Tasks found in this category.\n";
            }
        }else if(choice == 6){
            std::string today = Get_Today_Data();
            std::cout << "📆 Today is: " << today << "\n";
            bool found = false;
            std::cout << "\n_🔴 Overdue Task_\n";
            for(size_t i = 0 ; i < Tasks.size();i++){
            if(!Tasks[i].is_Done() && Tasks[i].get_Deadline()< today ){
                Tasks[i].show(i);
                found = true;
            }
            }
            if (!found) {
                 std::cout << "📭 No overdue Tasks.\n";
             }
            
        
        }else if (choice == 7) {
            int index;
            std::cout << "Enter task number to edit: ";
            std::cin >> index;
            std::cin.ignore();

            if (index < 0 || index >= (int)Tasks.size()) {
                std::cout << "❌ Invalid index.\n";
            } else {
                std::string title, desc, category, deadline;
                std::cout << "Leave field empty if you don't want to change it.\n";

                std::cout << "New title (" << Tasks[index].get_Title() << "): ";
                std::getline(std::cin, title);
                if (!title.empty()) Tasks[index].set_Title(title);

                std::cout << "New description (" << Tasks[index].get_Description() << "): ";
                std::getline(std::cin, desc);
                if (!desc.empty()) Tasks[index].set_Description(desc);

                std::cout << "New category (" << Tasks[index].get_Category() << "): ";
                std::getline(std::cin, category);
                if (!category.empty()) Tasks[index].set_Category(category);

                std::cout << "New deadline (" << Tasks[index].get_Deadline() << "): ";
                std::getline(std::cin, deadline);
                if (!deadline.empty()) Tasks[index].set_Deadline(deadline);

                std::cout << "✅ Task updated.\n";
            }

        }else if(choice == 8){
            Export_To_Html(Tasks,"Tasks.html");
        }else if(choice == 9){
            std::sort(Tasks.begin(),Tasks.end(),[](const Task & a, const Task & b){
                return a.get_Deadline() < b.get_Deadline();
            });
            std::cout << "✅ Tasks sorted by deadline.\n";
            Task::show_All(Tasks);
        }else if(choice == 10){
            std:sort(Tasks.begin(),Tasks.end(),[](const Task& a, const Task & b){
                return a.is_Done() < b.is_Done();
            });
            std::cout << "✅ Tasks sorted by status.\n";
            Task::show_All(Tasks);

        }else if (choice == 11){
            std::string keyword;
            std::cout << "Enter keyword to search : ";
            std::getline(std::cin,keyword);

            if(keyword.empty()){
                 std::cout << "❌ Keyword cannot be empty.\n";    
            }else{
                std::cout << "🔍 Search results for: \"" << keyword << "\"\n";

                bool found = false; 
                for(size_t i = 0 ; i < Tasks.size(); ++i){
                    const Task  & t  = Tasks[i];
                    if( t.get_Title().find(keyword) != std::string::npos ||
                        t.get_Description().find(keyword) != std::string::npos ||
                        t.get_Category().find(keyword) != std::string::npos ){
                            t.show(i);
                            found = true;
                        }
                }
                if(!found){
                    std::cout << "😕 No tasks found with keyword \"" << keyword << "\"\n";
                }
            }
        }else if (choice == 12){
            int done_count = 0;
            int not_done_count = 0;

            for(const auto & task : Tasks){
                if(task.is_Done()) 
                done_count++;
                else
                not_done_count++;
            }
            int total = done_count + not_done_count ; 
            double percent_done = total > 0 ? ((double)done_count / total) * 100 : 0;

            std::cout << "\n📊 Task Statistics\n";
            std::cout << "------------------\n";
            std::cout << "📋 Total Tasks: " << total << "\n";
            std::cout << "✅ Done: " << done_count << "\n";
            std::cout << "❌ Not Done: " << not_done_count << "\n";
            std::cout << "📈 Completion: " << percent_done << "%\n\n";
        }
        


    }while(choice != 13);
    
        
     saveTasks(Tasks);
     std::cout << "👋 Bye!\n";


    return 0;
}
