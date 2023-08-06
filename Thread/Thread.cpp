#include <iostream>
#include <thread>
#include <mutex>
#include <chrono>
#include <vector>

using namespace std;

size_t g_i = 0;

void Test()
{
    mutex m;
    m.lock();
    for (size_t i = 0; i < 1000000; i++)
    {
        ++g_i;
    }
    
    m.unlock();
}

void MakeThread(vector<thread>* pVThread, int count)
{
    for (int i = 0; i < count; ++i)
    {
        pVThread->push_back(thread(Test));
    }
}

int main()
{
    vector<thread> vThread;
    MakeThread(&vThread, thread::hardware_concurrency());

    //cout << thread::hardware_concurrency() << endl;

    // 시작 시간 측정
    auto startTime = chrono::high_resolution_clock::now();

    /*for (int i = 0; i < vThread.size(); ++i)
    {
        vThread[i] = thread
    }*/

    for (int i = 0; i < vThread.size(); ++i)
    {
        vThread[i].join();
    }

    // 종료 시간 측정
    auto endTime = chrono::high_resolution_clock::now();

    // 실행 시간 계산
    auto duration = chrono::duration_cast<chrono::microseconds>(endTime - startTime).count();

    // 결과 출력
    cout << "성능 평가에 걸린 시간: " << duration << " 마이크로초" << endl;

    cout << g_i << endl;
}