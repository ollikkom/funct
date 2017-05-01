#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <numeric>

enum Grade
{
    F = 1,
    D,
    C,
    B,
    A
};

struct Student
{
    std::string FIO;
    std::string Group;
    std::map<std::string, Grade> Grades;
};

std::vector<Student> LoadStudents()
{
    return {
            {
                    "Anna", "G7",
                    {
                            {"math", Grade::A},
                            {"cs", Grade::A},
                            {"english", Grade::A},
                            {"dutch", Grade::B},
                    }
            },
            {
                    "Petr", "G7",
                    {
                            {"math", Grade::A},
                            {"cs", Grade::B},
                            {"dutch", Grade::C},
                    }
            },
            {
                    "Vova", "G1",
                    {
                            {"math", Grade::F},
                            {"english", Grade::F},
                    }
            },
            {
                    "Vasya", "G3",
                    {
                            {"math", Grade::F},
                            {"english", Grade::F},
                            {"cs", Grade::B},
                            {"dutch", Grade::C},
                    }
            },
            {
                    "Anton", "G4",
                    {
                            {"math", Grade::F},
                            {"english", Grade::F},
                            {"dutch", Grade::C},
                            {"cs", Grade::C},
                    }
            },
            {
                    "Masha", "G2",
                    {
                            {"math", Grade::A},
                    }
            },
            {
                    "Sasha", "G6",
                    {
                            {"english", Grade::F},
                            {"russian", Grade::F},
                            {"dutch", Grade::F},
                    }
            },
            {
                    "Jeka", "G2",
                    {
                            {"english", Grade::A},
                    }
            },
            {
                    "Sveta", "G3",
                    {
                    }
            }
    };
}


struct FindStudentGroup
{
    std::string neededGroup;
    bool operator()(const Student& s) const
    {
        return s.Group == neededGroup;
    }
};

struct PrinterStudent
{
    void operator()(const Student& s) const
    {
        std::cout << s.FIO << " studies in " << s.Group << std::endl;
    }
};


struct GradeEqual
{
    Grade grade = Grade::A;

    bool operator()(const std::pair<std::string, Grade> & el) const
    {
        return el.second == grade;
    }
};

struct SrGrade
{
    std::string info;
    float Sr=0;
    void operator()(Student& a){
        int s = 0;
        size_t i = 0;
        for (auto it = a.Grades.begin(); it != a.Grades.end(); ++it){
            s += it->second;
            ++i;
        }
        if (i != 0 && (Sr > s / i || Sr == 0)) {
            info= a.FIO;
            Sr = s / i;
        }
    }
};

struct FGroup
{
    std::multimap<std::string, Student> people;
    void operator()(Student& a){
        people.insert(std::make_pair(a.Group, a));
    }
};

struct Marks {
    std::vector<Student> mark;
    void operator()(Student &a) {
        auto s1 = a.Grades.find("math"), s2 = a.Grades.find("cs");
        if (s1 != a.Grades.end() && s2 != a.Grades.end()) {
            if (s1->second == Grade::B && s2->second == Grade::A)
                mark.push_back(a);
        }
    }
};

struct Dutch{
    void operator()(Student &a){
        auto s = a.Grades.find("dutch");
        if (s != a.Grades.end()) {
            std::cout << a.FIO << " Marks: " << s->second << std::endl;
        }
        else {
            std::cout << a.FIO << " Does not study dutch " <<  std::endl;
        }
    }
};



int main()
{
    std::vector<Student> students = LoadStudents();

    // математика
    auto findM = std::max_element(students.begin(), students.end(),
                                  [](const Student & a, const Student & b)
                               {
                                   if (a.Grades.find("math") != a.Grades.end() && b.Grades.find("math") != b.Grades.end())
                                   {
                                       return a.Grades.at("math") < b.Grades.at("math");
                                   }
                                   else return false;
                               }
    );
    std::cout << findM->FIO << std::endl;

    // средняя оценка
    auto Sr = std::for_each(students.begin(), students.end(), SrGrade());
    std::cout << Sr.info << std::endl;

    // сколько предметов изучают
    auto FindMM = std::max_element(students.begin(), students.end(),
                               [](const Student & a, const Student & b)
                               {
                                   return a.Grades.size() < b.Grades.size();
                               }
    );
    std::cout << FindMM->FIO << std::endl;

    // поиск группы
    auto gr = std::for_each(students.begin(), students.end(), FGroup());

    // знают math на B, а cs на A
    auto MandCS = std::for_each(students.begin(), students.end(), Marks());


    // dutch
    std::for_each(students.begin(), students.end(), Dutch());
    return 0;

}