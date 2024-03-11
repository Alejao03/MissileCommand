#include "RenderThread.h"
#include <iostream>
#include "Renderer.h"
#include <mutex>
#include <condition_variable>

void RenderThread::start() {
	_hilo = std::thread(&RenderThread::main, this);

	//dado que main es un miembro de la clase necesitas pasarle una instancia de la misma donde ejecutar el hilo
}

void RenderThread::main() {
	// Lock used in the condition_variable to not make an active wait
	std::unique_lock<std::mutex> lock(_mutex);

	while (!_acabar) {
		condition_variable.wait(lock, [this] { return !(_commands.size_approx() <= 0) || _acabar; });
		Command comando = {};
		if (_commands.try_dequeue(comando)) {
			switch (comando.type) {
			case CLEAR:
				Renderer::Instance()->clear(comando.color);
				break;
			case DRAW_SPRITE: {
				const Sprite* sprite = comando.spriteParam.sprite;

				const Vector2 pos = comando.spriteParam.pos;
				const int width = comando.spriteParam.width;
				const int height = comando.spriteParam.height;

				if (sprite != nullptr)
					sprite->draw(pos, width, height);
			}
							break;
			case DRAW_RECT: {
				const Vector2 pos = comando.rectParam.pos;
				const int width = comando.rectParam.width;
				const int height = comando.rectParam.height;
				const uint32_t color = comando.rectParam.color;
				const bool fill = comando.rectParam.fill;
				Renderer::Instance()->drawRect(pos.getX(), pos.getY(), width, height, color, fill);
			}
						  break;
			case DRAW_LINE: {
				const Vector2 startPos = comando.lineParam.startPos;
				const Vector2 pos = comando.lineParam.pos;
				const uint32_t color = comando.lineParam.color;

				Renderer::Instance()->drawLine(startPos.getX(), startPos.getY(), pos.getX(), pos.getY(), color);
			}
						  break;
			case DRAW_CIRCLE: {
				const Vector2 pos = comando.circleParam.pos;
				const uint32_t color = comando.circleParam.color;
				const int r = comando.circleParam.r;

				Renderer::Instance()->drawCircle(pos.getX(), pos.getY(), r, color);
			}
							break;
			case PRESENT:
				_pendingFrames--;
				Renderer::Instance()->present();
				break;
			case STOP:
				_acabar = true;
				break;
			default:
				std::cout << "UNKNOWN COMMAND in RenderThread::main()" << std::endl;
				break;
			}
		}
	}
}

void RenderThread::enqueCommand(Command comando) {
	if (comando.type == PRESENT)
		_pendingFrames++;

	_commands.emplace(comando);
	condition_variable.notify_one(); // Notify the thread that there is a new command
}

void RenderThread::stop() {
	//mandar mensaje----
	Command comando = {};
	comando.type = STOP;
	enqueCommand(comando);
	_hilo.join();
}

int RenderThread::pendingFrame() const
{
	return _pendingFrames;
}
