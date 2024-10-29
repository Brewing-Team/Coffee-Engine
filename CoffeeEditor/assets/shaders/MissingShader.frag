#version 450 core
layout(location = 0) out vec4 FragColor;
layout(location = 1) out vec4 EntityID;

uniform vec3 entityID;

void main()
{
    FragColor = vec4(vec3(1.0, 0.0, 1.0), 1.0);
    EntityID = vec4(entityID, 1.0f); //set the alpha to 0
}
