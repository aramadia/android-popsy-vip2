/*-------------------------------------------------------------+
 |                       Scheduler.h                            |
 |                                                              |
 |  c'est lui ki va réguler le lancement des différentes taches |
 |                                                              |
 |  U2^PoPsy TeAm 2000                                      |
 +-------------------------------------------------------------*/

#ifndef _SCHEDULER_H_
#define _SCHEDULER_H_


class UScheduler;


//////////////////////////////////////////
// definit une tache du scheduler

typedef void (*PROCUTASK)(UScheduler* sche, U32 user);

class UTask {

//--------- Datas
public:

    CString     nom;
    PROCUTASK   procs;
    bool        suspendu;   // la tache est-elle suspendue ?
    bool        mustDie;    // la tache doit mourirrr !
    UTask*      suiv;

//--------- Fonctions
public:

    UTask();

};
//////////////////////////////////////////




//////////////////////////////////////////
// definit le Scheduler lui meme
class UScheduler {

//----------- Datas
private:

    UTask*   AllTask;
    UTask*   LastTask;
    UTask*   ActuaTask;

//----------- Fonctions
private:

    UTask*   Find(LPCSTR nom);
    void    SetLast();

public:

    UScheduler();
    ~UScheduler();

    void AddTask(LPCSTR nom, PROCUTASK  procs);

    void SuspendTask(LPCSTR nom);
    void SuspendAllTask();
    void ReScheduleTask(LPCSTR nom);
    void ReScheduleAllTask();

    void KillActuaTask();
    void KillTask(LPCSTR nom);
    void KillAllDeadTask();

    void Run();

};
//////////////////////////////////////////

#endif
