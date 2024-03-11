#pragma once
#include <cstdint>
#include <thread>
#include "Command.h"

//#include <queue>
#include "readerwriterqueue.h"
#include <mutex>
#include <condition_variable>

class RenderThread
{
private:
	std::thread _hilo;
	moodycamel::ReaderWriterQueue<Command> _commands;
	volatile int _pendingFrames = 0;
	volatile bool _acabar = false;
	std::mutex _mutex;
	std::condition_variable condition_variable;

	void main();

public:
	/// <summary>
	/// Lanza el hilo
	/// </summary>
	void start();
	/// <summary>
	/// Encola el comando que se le da
	/// </summary>
	/// <param name="comando">comando a encolar</param>
	void enqueCommand(Command comando);
	/// <summary>
	/// Termina el hilo
	/// </summary>
	void stop();
	/// <summary>
	/// Cuantos frames quedan pendientes de diujar
	/// </summary>
	/// <returns></returns>
	int pendingFrame() const; 
};

