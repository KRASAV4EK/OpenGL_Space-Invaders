# 🎮 **Space Invaders Remastered**

A [modernized remake](/remaster/) of a [broken legacy prototype](/prototype/), rewritten into a fully playable arcade shooter.

## 👾 Prototype
![pic](/pics/prototype.png)

## 🚀 Remaster
![pic](/pics/remaster.png)

## 📌 ***About the Project***

This project began as a deliberately incomplete and chaotic prototype.

### **The original codebase had**:

❌ No collision detection

❌ No score or highscore system

❌ No explosion effects

❌ No object architecture

❌ Poor formatting and almost no documentation

My task was to rebuild the entire game, turning a barely functioning prototype into a complete, polished, maintainable arcade shooter suitable for further development.

### **Today, the project represents**:

✨ A cleaned and structured codebase

✨ A fully playable Space-Invaders-style game

✨ Good-quality documentation and architecture

✨ Extensible gameplay and rendering systems

## 🧱 ***Core Features Implemented***

### ✔ **Complete Object-Oriented Architecture**

The game was rewritten around a clear and maintainable OOP structure:

**Object** – base class for all entities\
**User** – player ship logic\
**Enemy** – fully AI-driven formation\
**Bullet** – pooled projectile system\
**Explosion** – reusable animation module\
**Score** – scoring & highscore persistence\
**GameSettings** – centralized configuration\
**Collision** – collision routing system

This fully replaces the previous spaghetti code.

### 💥 ***Gameplay Systems*** 

🔹 **Player Controller**
- Smooth horizontal movement
- Tilt animation
- Wobble effect
- Bullet firing with cooldown
- Multiple lives displayed via icons
- Player explosion animation

🔹 **Enemy Formation AI**
- Horizontal marching with global direction control
- Vertical descent
- Per-enemy bullet
- Firing probability that increases over time
- Tint color per row
- Automatic wave reset when all enemies die
- Difficulty scaling

🔹 **Projectiles & Collisions**
- Bullet pooling (player + enemy)
- Circle–circle collision detection
- Bullet-vs-enemy
- Player-vs-enemy
- Player-vs-enemy-bullet
- Bullet-vs-bullet
- Explosion triggering on impact

### 🎆 ***Visual Systems***

🔹 **Sprite Rendering via leetlib**
- Rotations, scaling, tinting
- Texture loading & management
- Alpha blending & filtering
- Frame-based explosion effects (33 frames)
- Animated title
- Animated background

🔹 **Other systems**
- Real-time score display
- Persistent highscore saved to disk
- End-of-game restart prompt
- Animated SPACE INVADERS title
- Player life icons
- Final highscore message after quitting

## 📂 ***Project Architecture***

### /**Headers**
    - Object.h
    - User.h
    - Enemy.h
    - Bullet.h
    - Explosion.h
    - Score.h
    - GameSettings.h
    - Collision.h

### /**Source**
    - Object.cpp
    - User.cpp
    - Enemy.cpp
    - Bullet.cpp
    - Explosion.cpp
    - Score.cpp
    - GameSettings.cpp
    - Collision.cpp
    - main.cpp

### /**lib**
    - leetlib.cpp
    - leetlib.h


Everything is modular, documented, and ready for extension.

## 🛠️ ***Technologies Used***

- C++
- Direct3D9 (via legacy leetlib)
- FMOD — audio playback
- Win32 API — windowing, input, messaging
- Sprite-based 2D rendering
- Object pooling
- Custom animation system

## 🧭 ***Improvements Over the Original Prototype***

### **The original file was a single messy loop with**:

- No architecture
- No memory reuse
- Hardcoded values everywhere
- Random movement without logic
- No state management
- No separation of game systems

### **The new version includes**:

✔ Full modularization\
✔ Clean class hierarchy\
✔ Shared static systems for formation behavior\
✔ High-quality English documentation\
✔ Maintainability suitable for a real studio


## 🏁 ***Final Result***

### **This project demonstrates my ability to**:

- Refactor and modernize old/chaotic code
- Design gameplay systems and architecture
- Work with graphics, input, and audio at a low level
- Produce maintainable, documented code suitable for production
- The final game is a complete Space Invaders remake with improved visuals, gameplay, and code quality.

# ***Thanks!***

I hope this project provides a clear view of my game-development and software-engineering skill set.

Total time spent: ~40 hours.\
Total commits created: 54.
