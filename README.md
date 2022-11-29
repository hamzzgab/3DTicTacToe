# 3D TicTacToe

The game is created on a 4 by 4 grid with two markers, ‘X’ and ‘O’. The game’s objective is to place four Xs or four Os in one horizontal, vertical, or diagonal row. To do this, you may use one plane or all four planes.

<p align="center">
<img width="250" alt="Atari 3D TicTacToe" src="https://user-images.githubusercontent.com/47095611/204596196-cb1f4675-2e2a-452c-a5ea-34108b79c345.png">
</p>



<img src="https://user-images.githubusercontent.com/47095611/204605169-05e06325-e0f9-42e8-bd9d-194873090e3a.mp4" width="400">






## Projections
I created the game in an **orthographic projection**. Three models are used for this, i.e. ‘X’ and ‘O’ markers and the primary 4x4 grid. There are four 4x4 grids stacked on top of each other; in these blocks, the different markers are placed at their respective position.

<p align="center">
<img width="400" alt="OpenGL 3D TicTacToe" src="https://user-images.githubusercontent.com/47095611/204596701-c708df4f-5bb7-4cf4-9d93-f45c0613387f.png">
</p>

You can shift between perspectives by pressing the key 'P'. The default orientation is orthographic, on pressing 'P' it becomes into a perspective projection

<p align="center">
<img width="400" alt="Perspective Projection" src="https://user-images.githubusercontent.com/47095611/204597536-d722d7a5-1cdd-4f85-80d2-08c85bb097b4.png">
</p>

## Movement
The camera can be moved by using the Left (A), Right (D), Forward (W), Backward (S) keys. And also Left, Right, Up, and Down using the mouse.

### Key Movements
1. Left (Key A):
<p align="center">
<img width="400" alt="Left" src="https://user-images.githubusercontent.com/47095611/204598126-f8de1957-8528-4606-ba9a-0c3744859e8d.png">
</p>

2. Right (Key D):
<p align="center">
<img width="400" alt="Right" src="https://user-images.githubusercontent.com/47095611/204598253-bec8de45-9cf9-42d5-8faf-ee2e5cf795b0.png">
</p>

**Note: FORWARD AND BACKWARD TRANSFORMATIONS ARE ONLY VISIBLE IN PERSPECTIVE PROJECTION**

3. Forward (Key W):
<p align="center">
<img width="400" alt="Forward" src="https://user-images.githubusercontent.com/47095611/204598608-a9419a77-80d8-4c86-b575-9df8fa00611f.png">
</p>

5. Backward (Key S):
<p align="center">
<img width="400" alt="Backward" src="https://user-images.githubusercontent.com/47095611/204598696-9b8e5d46-d758-4af7-afe6-1fdddad8dd31.png">
</p>

### Scrolling

1. Up 
<p align="center">
  <img width="400" alt="Screenshot 2022-11-25 at 19 57 43" src="https://user-images.githubusercontent.com/47095611/204599403-973b77d9-f39f-46fe-8e7b-5fbdd5aacc72.png">
</p>
2. Down
<p align="center">
  <img width="400" alt="Screenshot 2022-11-25 at 19 57 36" src="https://user-images.githubusercontent.com/47095611/204599430-12de3ff3-1898-4e7a-b638-b27c3d396e86.png">
</p>
4. Left
<p align="center">
  <img width="400" alt="Screenshot 2022-11-25 at 19 57 32" src="https://user-images.githubusercontent.com/47095611/204599471-c4ffb06f-e0dc-41f4-ab56-84590b4caeb0.png">
</p>
5. Right
<p align="center">
  <img width="400" alt="Screenshot 2022-11-25 at 19 57 27" src="https://user-images.githubusercontent.com/47095611/204599498-de771b74-64f6-4c7a-a56e-ebd9bb4b2151.png">
</p>

## Lighting

I have implemented Directional Lighitng in the game. So that each element in the game has the same amout of lighting.


