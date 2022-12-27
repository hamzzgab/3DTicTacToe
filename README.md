# 3D TicTacToe
1. [Modeling & Textuing](#1-model-and-texturing)
2. [transforms](#2-transforms)
3. [Camera Views](#3-camera-views)
4. [Hierarchical Structures](#4-hierarchical-structures)
5. [Playing the Game](#5-playing-the-game)



<p align='center'>
  <a href="http://www.youtube.com/watch?feature=player_embedded&v=Uk0CfXhTHmM" target="_blank">
      <img src="http://img.youtube.com/vi/Uk0CfXhTHmM/0.jpg" alt="3D TicTacToe" width="240" height="180" border="10" />
  </a>
<p>

## 1. Modeling and Texturing
## 2. Transforms
  
```
glm::mat4 model_create(GLfloat grid_translate){
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate( model, glm::vec3( grid_translate, 0.0f, -2.5f ));
    model = glm::rotate(model, glm::radians(degreeRotation), glm::vec3(1.0f, 1.0f, 0.0f));
    model = glm::rotate(model, glm::radians(-18.5f), glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::scale(model, glm::vec3(scaling));
    return model;
}

```
  
`GLfloat grid_translate[] = {0.0f, 0.05f, 0.1f, 0.15f};`
  
```
glm::vec3 curr_grid[] = {
    glm::vec3( 0.0f, 60.0f, 0.0f ),
    glm::vec3( 0.0f, 20.0f, 0.0f ),
    glm::vec3( 0.0f, -20.0f, 0.0f ),
    glm::vec3( 0.0f, -60.0f, 0.0f ),
};
```
  
```
void createObject(const std::string& objectName, Shader *shader, Model *draw, glm::mat4 projection, glm::mat4 view, glm::mat4 model, glm::vec4 extra_color){
    glUniformMatrix4fv( glGetUniformLocation( shader->Program, "projection" ), 1, GL_FALSE, glm::value_ptr( projection ) );
    glUniformMatrix4fv( glGetUniformLocation( shader->Program, "view" ), 1, GL_FALSE, glm::value_ptr( view ) );
    glUniformMatrix4fv( glGetUniformLocation( shader->Program, "model" ), 1, GL_FALSE, glm::value_ptr( model ) );
    glUniform4f( glGetUniformLocation( shader->Program, "extra_color" ), extra_color.r, extra_color.g, extra_color.b, extra_color.a);
    draw->Draw(*shader);
}
```
  
| Key  | Movement |
| ------------- | ------------- |
| W & :arrow_up: | UP |
| A & :arrow_left: | LEFT |
| S & :arrow_down: | DOWN |
| D & :arrow_right: | RIGHT |
| Q | GRID DOWN | 
| E | GRID UP |
| Spacebar | Render Marker |
  
## 3. Camera Views
## 4. Hierarchical Structures
## 5. Playing the Game
