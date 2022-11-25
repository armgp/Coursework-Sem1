#include <bits/stdc++.h>

using namespace std;

int id = 0;

struct Process{
    int pid;
    int arrivalTime;
    int burstTime;
    int waitingTime;
    int completionTime;
    int turnAroundTime;

    Process(){
        pid = id++;
    }

    Process(int at, int bt){
        pid = id++;
        arrivalTime = at;
        burstTime = bt;
    }

    void showPocess(){
        cout<<"pid: "<<pid<<"\n";
        cout<<"arrivalTime: "<<arrivalTime<<"\n";
        cout<<"burstTime: "<<burstTime<<"\n";
        cout<<"waitingTime: "<<waitingTime<<"\n";
        cout<<"completionTime: "<<completionTime<<"\n";
        cout<<"turnAroundTime: "<<turnAroundTime<<"\n";
        cout<<"-------------------------------\n\n";
    }
};

void fcfs(vector<Process>& pList){
    sort(pList.begin(), pList.end(), [](Process &p1, Process &p2){
        return (p1.arrivalTime<p2.arrivalTime || (p1.arrivalTime==p2.arrivalTime && p1.pid<p2.pid));
    });

    int prevCt = -1;

    for(Process& p : pList){
        int currTime = max(p.arrivalTime, prevCt);
        currTime+=p.burstTime;
        p.completionTime = currTime;
        p.turnAroundTime = p.completionTime-p.arrivalTime;
        p.waitingTime = p.turnAroundTime-p.burstTime;
        prevCt = p.completionTime;
    }

}

struct compareBurstTime{
    bool operator()(Process &p1, Process &p2){
        return (p1.arrivalTime<p2.arrivalTime || (p1.arrivalTime==p2.arrivalTime && p1.pid<p2.pid));
    }
};

void sjf(vector<Process>& pList){

    sort(pList.begin(), pList.end(), [](Process &p1, Process &p2){
        return (p1.burstTime<p2.burstTime || (p1.burstTime==p2.burstTime && p1.pid<p2.pid));
    });

    priority_queue<Process, vector<Process>, compareBurstTime> minHeap;

    int currTime = pList[0].arrivalTime;
    for(int i=0 ; i<pList.size(); i++){

    }
}

int main(){

    int n;
    cin>>n;

    vector<Process> processes;

    while(n--){
        int at, bt;
        cin>>at>>bt;
        Process p(at, bt);
        processes.push_back(p);
    }

    fcfs(processes);

    for(Process p : processes) p.showPocess();
    
    return 0;
}