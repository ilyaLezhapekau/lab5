#include <Windows.h>
#include <iostream>
#include <time.h>
#include <cstring>

using namespace std;
int const arr_size = 10;
volatile LONG arr[arr_size];


void Thread_1() {

    HANDLE hMutex;
    if ((hMutex = OpenMutex(SYNCHRONIZE, FALSE, "Mutex")) == NULL) {
        int error = GetLastError();
        cerr << "Open Mutex Error " << error << endl;
        return;
    }
    WaitForSingleObject(hMutex, INFINITE);
    srand(time(NULL));
    rand();
    for (int i = 0; i < arr_size; i++) {
        arr[i] = rand() * 300 / (RAND_MAX) - 150;
    }
    cout << "Origin array " << endl;
    for (int i = 0; i < arr_size; i++) {
        cout << arr[i] << " ";
    }
    cout << endl;
    ReleaseMutex(hMutex);
}


void Thread_2(){
    HANDLE hMutex;
    if((hMutex = OpenMutex(SYNCHRONIZE, FALSE, "Mutex"))== NULL){
        int error = GetLastError();
        cerr<<"Open Mutex Error "<<error<<endl;
        return;
    }
    WaitForSingleObject(hMutex, INFINITE);
    srand(time(NULL));
    rand();
    for( int i = 0; i< arr_size; i++){
        if((arr[i]>=10 and arr[i]<=99) or (arr[i]>=-99 and arr[i]<=-10)){
            arr[i] =0;
        }
    }
    cout<<"new array "<<endl;
    for( int i = 0; i< arr_size; i++) {
        cout << arr[i] << " ";
    }
    ReleaseMutex(hMutex);
}

int main() {
    HANDLE hMutex;
    if((hMutex = CreateMutex(NULL, FALSE, "Mutex")) == NULL){
        int error = GetLastError();
        cerr<<"Open Mutex Error "<<error<<endl;
        return error;
    }
    HANDLE hThread1, hThread2;
    DWORD IDThread_1, IDThread_2;
    if ((hThread1 = CreateThread(NULL, 0,(LPTHREAD_START_ROUTINE)Thread_1, NULL, 0, &IDThread_1))==NULL){
         int error = GetLastError();
         cout<<"error create Thread 1 "<<error<<endl;
         CloseHandle(hMutex);
         return error;
    }
    if ((hThread2 = CreateThread(NULL, 0,(LPTHREAD_START_ROUTINE)Thread_2, NULL, 0, &IDThread_2))==NULL) {
        int error = GetLastError();
        cout << "error create Thread 2 " << error << endl;
        CloseHandle(hThread1);
        CloseHandle(hMutex);
        return error;
    }
    WaitForSingleObject(hThread1, INFINITE);
    WaitForSingleObject(hThread2, INFINITE);
    CloseHandle(hThread1);
    CloseHandle(hThread2);
    CloseHandle(hMutex);
    return 0;

}