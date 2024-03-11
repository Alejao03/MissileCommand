#include "Animator.h"
#include "Renderer.h"
#include "Image.h"
#include "Sprite.h"

Animator::Animator(const Image* image, int numRows, int numCols, float frameDuration, int startCell, int endCell) {
    if (endCell == 0)endCell = numRows * numCols - 1;
    animationFrames = loadAnimation(image, numRows, numCols, startCell, endCell);
    _currentSprite = animationFrames[0]; // Copy constructor
    _currentFrame = 0;
    this->_frameDuration = frameDuration;
    _timer = 0.0f;
    _canplay = false;
}

Animator::~Animator() {
    for (Sprite* frame : animationFrames) {
        delete frame;
        frame = nullptr;
    }
}

void Animator::update(float deltaTime) {
   
    if (_canplay) //Solo se reproducce si se activa el trigger de reproduccion
    {
        _timer += deltaTime;
        if (_timer > _frameDuration) {
            _timer= 0.0f;
            _currentFrame = (_currentFrame + 1) % animationFrames.size();
            updateCurrentSprite();
            if (_currentFrame == animationFrames.size() - 1) {
                _canplay = false;  // Desactiva la reproducción si es el último frame
            }
        }
    }  
}

void Animator::restartAnimation() {
    _canplay = true;
    _currentFrame = 0;
    _timer = 0.0f;
}

void Animator::reset()
{
    _canplay = false;
    _currentFrame = 0;
    _timer = 0.0f;
    updateCurrentSprite();
}

Sprite* Animator::getCurrentSprite()
{
    return _currentSprite;
}

//Crear Sprites desde un Spritesheet
std::vector<Sprite*> Animator::loadAnimation(const Image* image, int numRows, int numCols, int startCell, int endCell)
{
    int spriteWidth = image->getWidth() / numCols;
    int spriteHeight = image->getHeight() / numRows;

    std::vector<Sprite*> animationFrames;

    for (int cell = startCell; cell <= endCell; ++cell) {
        float row = cell / numCols;
        float col = cell % numCols;
        float xImage = col * spriteWidth;
        float yImage = row * spriteHeight;

        Sprite* frame = new Sprite(image, { xImage, yImage }, spriteWidth, spriteHeight);
        animationFrames.push_back(frame);
    }

    return animationFrames;
}

void Animator::updateCurrentSprite() {
    Sprite* nextFrame = animationFrames[_currentFrame];
    _currentSprite = nextFrame;
}

void  Animator::play()
{
    _canplay = true;
}