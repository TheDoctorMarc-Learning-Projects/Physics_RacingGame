
 ### Welcome to our Radio Control physics game README 

Developed by CITM students: 

- José Antonio Prieto - GitHub: https://github.com/peterMcP
- Marc Doctor Pedrosa - GitHub: https://github.com/thedoctormarc
- Link to the source code and release of our game in Github: 
https://github.com/thedoctormarc/Physics_RacingGame


## Description
- The game is focused on a Radio Control car, 
which has to go through a series of obstacles 
like balls, wrecking ball, ramps, jumps, "fallingSnakes". Can you beat it?
 
### Objective to win 
- You need to do 3 laps in 3 minutes

### Normal Controls

- Arrow up: Accelerate the rc car.
- Arrow down: Brake/reverse (be careful, radio control cars spin very easy)
- Arrow left/right: direction
- Key P: open the rear wing (DRS) and gain speed
- Key R: reposition car on last reached checkpoint (adds a 10sec penalization time to your current lap time)
- Key N: start new game

### Debug Keys

- F1: to show/unshow body colliders (not disponible on release version, only shows world constraints)
- F3: locks/unlocks camera to vehicle
  - Move freely with WASD keys + Shift key to move faster
- F9: go directly to win condition

### Bullet3D physics constraints used
- Hinge: for full wrecking ball
- Point to point: to falling snakes inside tunnel

### Credits/References

- Backtrack music copyrighted by "Rise Against"
- All sfx from Humble RPG Game Dev Bundle - ProSounds collection - Nov2018

- Camera documentation/inspiration: https://forum.unity.com/threads/smooth-follow-behind-a-car.104504/
- Extra opengl lights info: http://what-when-how.com/opengl-programming-guide/creating-light-sources-opengl-programming/
