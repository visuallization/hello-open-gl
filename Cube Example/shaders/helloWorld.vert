#version 330

layout (std140) uniform Matrices {
	mat4 pvm;
};

uniform mat4 translation = mat4(
    vec4(1.0,   0.0,    0.0,    0.0),
    vec4(0.0,   1.0,    0.0,    0.0),
    vec4(0.0,   0.0,    1.0,    0.0),
    vec4(0.0,   0.0,    0.0,    1.0)
);

uniform float angle = 0;

// Rotation around x axis
//mat4  rotation = mat4(
//    vec4( 1.0,           0.0,               0.0,            0.0 ),
//    vec4( 0.0,	         cos(angle),        -sin(angle),    0.0 ),
//    vec4( 0.0,           sin(angle),        cos(angle),     0.0 ),
//    vec4( 0.0,           0.0,               0.0,            1.0 )
//);

// Rotation around y axis
mat4  rotation = mat4(
    vec4( cos(angle),   0.0,        sin(angle),     0.0 ),
    vec4( 0.0,	      1.0,        0.0,            0.0 ),
    vec4( -sin(angle),  0.0,        cos(angle),     0.0 ),
    vec4( 0.0,          0.0,        0.0,            1.0 )
);

// Rotation around z axis
//mat4  rotation = mat4(
//    vec4( cos(angle),     -sin(angle),      0.0,     0.0 ),
//    vec4( sin(angle),	    cos(angle),       0.0,     0.0 ),
//    vec4( 0.0,            0.0,              1.0,     0.0 ),
//    vec4( 0.0,            0.0,              0.0,     1.0 )
//);

in vec4 position;

out vec4 color;

void main()
{
	color = position;

    mat4 center = mat4(
        vec4(1.0,   0.0,    0.0,    0.0),
        vec4(0.0,   1.0,    0.0,    0.0),
        vec4(0.0,   0.0,    1.0,    0.0),
        vec4(0.5,   0.5,    0.5,    1.0)
    );

    mat4 recenter = mat4(
        vec4(1.0,   0.0,    0.0,    0.0),
        vec4(0.0,   1.0,    0.0,    0.0),
        vec4(0.0,   0.0,    1.0,    0.0),
        vec4(-0.5,   -0.5,    -0.5,    1.0)
    );

    vec4 transformed =  translation * center * rotation * recenter * position;

	gl_Position = pvm * transformed;
} 
