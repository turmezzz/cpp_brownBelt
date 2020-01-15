#include <iostream>
#include <map>
#include <string>
#include <tuple>

using namespace std;

// Перечислимый тип для статуса задачи
enum class TaskStatus {
    NEW,          // новая
    IN_PROGRESS,  // в разработке
    TESTING,      // на тестировании
    DONE          // завершена
};

// Объявляем тип-синоним для map<TaskStatus, int>,
// позволяющего хранить количество задач каждого статуса
using TasksInfo = map <TaskStatus, int>;

class TeamTasks {
private:
    map <string, TasksInfo> person_2_tasks;

public:
    // Получить статистику по статусам задач конкретного разработчика
    const TasksInfo& GetPersonTasksInfo(const string& person) const {
        return person_2_tasks.find(person)->second;
    }

    // Добавить новую задачу (в статусе NEW) для конкретного разработчитка
    void AddNewTask(const string& person) {
        ++person_2_tasks[person][TaskStatus::NEW];
    }

    // Обновить статусы по данному количеству задач конкретного разработчика,
    // подробности см. ниже
    tuple <TasksInfo, TasksInfo> PerformPersonTasks(
            const string& person, int task_count) {
        if (person_2_tasks.find(person) == person_2_tasks.end()) {
            return {{}, {}};
        }

        auto& person_tasks = person_2_tasks.find(person)->second;
        task_count = min(task_count, person_tasks[TaskStatus::NEW] + person_tasks[TaskStatus::IN_PROGRESS] +
                                     person_tasks[TaskStatus::TESTING] + person_tasks[TaskStatus::DONE]);

        int new_to_in_progress = min(person_tasks[TaskStatus::NEW], task_count);
        person_tasks[TaskStatus::NEW] -= new_to_in_progress;
        task_count -= new_to_in_progress;

        int in_progress_to_testing = min(person_tasks[TaskStatus::IN_PROGRESS], task_count);
        person_tasks[TaskStatus::IN_PROGRESS] -= in_progress_to_testing;
        task_count -= in_progress_to_testing;

        int testing_to_done = min(person_tasks[TaskStatus::TESTING], task_count);
        person_tasks[TaskStatus::TESTING] -= testing_to_done;
        task_count -= testing_to_done;

        TasksInfo not_updated_tasks;
        if (person_tasks[TaskStatus::NEW] != 0) {
            not_updated_tasks[TaskStatus::NEW] = person_tasks[TaskStatus::NEW];
        }
        if (person_tasks[TaskStatus::IN_PROGRESS] != 0) {
            not_updated_tasks[TaskStatus::IN_PROGRESS] = person_tasks[TaskStatus::IN_PROGRESS];
        }
        if (person_tasks[TaskStatus::TESTING] != 0) {
            not_updated_tasks[TaskStatus::TESTING] = person_tasks[TaskStatus::TESTING];
        }

        TasksInfo updated_tasks;
        if (new_to_in_progress != 0) {
            updated_tasks[TaskStatus::IN_PROGRESS] = new_to_in_progress;
        }
        if (in_progress_to_testing != 0) {
            updated_tasks[TaskStatus::TESTING] = in_progress_to_testing;
        }
        if (testing_to_done != 0) {
            updated_tasks[TaskStatus::DONE] = testing_to_done;
        }

        person_tasks[TaskStatus::NEW];
        person_tasks[TaskStatus::IN_PROGRESS] += new_to_in_progress;
        person_tasks[TaskStatus::TESTING] += in_progress_to_testing;
        person_tasks[TaskStatus::DONE] += testing_to_done;
        return {updated_tasks, not_updated_tasks};
    }
};


// Принимаем словарь по значению, чтобы иметь возможность
// обращаться к отсутствующим ключам с помощью [] и получать 0,
// не меняя при этом исходный словарь
void PrintTasksInfo(TasksInfo tasks_info) {
    cout << tasks_info[TaskStatus::NEW] << " new tasks" <<
         ", " << tasks_info[TaskStatus::IN_PROGRESS] << " tasks in progress" <<
         ", " << tasks_info[TaskStatus::TESTING] << " tasks are being tested" <<
         ", " << tasks_info[TaskStatus::DONE] << " tasks are done" << endl;
}


int main() {
    TeamTasks tasks;
    tasks.AddNewTask("Ilia");
    for (int i = 0; i < 3; ++i) {
        tasks.AddNewTask("Ivan");
    }
    cout << "Ilia's tasks: ";
    PrintTasksInfo(tasks.GetPersonTasksInfo("Ilia"));
    cout << "Ivan's tasks: ";
    PrintTasksInfo(tasks.GetPersonTasksInfo("Ivan"));

    TasksInfo updated_tasks, untouched_tasks;

    tie(updated_tasks, untouched_tasks) =
            tasks.PerformPersonTasks("Ivan", 2);
    cout << "Updated Ivan's tasks: ";
    PrintTasksInfo(updated_tasks);
    cout << "Untouched Ivan's tasks: ";
    PrintTasksInfo(untouched_tasks);

    tie(updated_tasks, untouched_tasks) =
            tasks.PerformPersonTasks("Ivan", 2);
    cout << "Updated Ivan's tasks: ";
    PrintTasksInfo(updated_tasks);
    cout << "Untouched Ivan's tasks: ";
    PrintTasksInfo(untouched_tasks);

    return 0;
}
