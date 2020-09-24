#version 460 core
out vec4 FragColor;

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
}; 

struct DirLight {
    vec3 direction;
	
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

uniform vec3 viewPos;
uniform DirLight dirLight;
uniform Material grass;
uniform Material earth;
uniform Material rock;
uniform Material snow;
uniform vec2 heightParams; //x - min height, y - max height

// function prototypes
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);

void main()
{    
    // properties
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);
    
    // phase 1: directional lighting
    vec3 result = CalcDirLight(dirLight, norm, viewDir);
    result *= 1.1f; //Amplify directional light influence

    FragColor = vec4(result, 1.0);
}

// calculates the color when using a directional light.
/* 
Fragment positions are normalised to [0, 1].
Textures are applied on an absolute(One texture) and blend(Blend with upper and lower level texture) level.
[0.95-1.0] Snow
[0.85-0.95] Snow/Rock blend
[0.75-0.85] Rock
[0.65-0.75] Rock/Grass blend
[0.30-0.65] Grass 
[0.15-0.0.30] Grass/Earth blend
[0.0 - 0.15] Earth
*/
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
    float normalisedHeight = ((FragPos.y - heightParams.x) / (heightParams.y - heightParams.x));
    //normalisedHeight = 1 - normalisedHeight;
    vec3 lightDir = normalize(-light.direction);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float mixRatio;
    float spec;
    vec3 ambient, diffuse, specular;
    //float spec = pow(max(dot(viewDir, reflectDir), 0.0), grass.shininess);
    if(normalisedHeight >= 0.95){ //SNOW
        spec = pow(max(dot(viewDir, reflectDir), 0.0), snow.shininess);     
        ambient = light.ambient * vec3(texture(snow.diffuse, TexCoords));
        diffuse = light.diffuse * diff * vec3(texture(snow.diffuse, TexCoords));
        specular = light.specular * spec * vec3(texture(snow.specular, TexCoords));
    }
    else if(normalisedHeight >= 0.85 && normalisedHeight <= 0.95){ //SNOW/ROCK MIX
        mixRatio = ((normalisedHeight - 0.85) / (0.95 - 0.85));
        spec = pow(max(dot(viewDir, reflectDir), 0.0), rock.shininess);     
        ambient = light.ambient * vec3(mix(texture(rock.diffuse, TexCoords), texture(snow.diffuse, TexCoords), mixRatio));
        diffuse = light.diffuse * diff * vec3(mix(texture(rock.diffuse, TexCoords), texture(snow.diffuse, TexCoords), mixRatio));
        specular = light.specular * spec * vec3(mix(texture(rock.specular, TexCoords), texture(snow.specular, TexCoords), mixRatio));
    }
    else if(normalisedHeight >= 0.60 && normalisedHeight <= 0.85){ //ROCK/GRASS MIX
        mixRatio = ((normalisedHeight - 0.60) / (0.85 - 0.60));
        spec = pow(max(dot(viewDir, reflectDir), 0.0), grass.shininess);     
        ambient = light.ambient * vec3(mix(texture(grass.diffuse, TexCoords), texture(rock.diffuse, TexCoords), mixRatio));
        diffuse = light.diffuse * diff * vec3(mix(texture(grass.diffuse, TexCoords), texture(rock.diffuse, TexCoords), mixRatio));
        specular = light.specular * spec * vec3(mix(texture(grass.specular, TexCoords), texture(rock.specular, TexCoords), mixRatio));
    }
    else if(normalisedHeight >= 0.15 && normalisedHeight <= 0.60){ //GRASS/EARTH MIX
        mixRatio = ((normalisedHeight - 0.15) / (0.60 - 0.15));
        spec = pow(max(dot(viewDir, reflectDir), 0.0), earth.shininess);     
        ambient = light.ambient * vec3(mix(texture(earth.diffuse, TexCoords), texture(grass.diffuse, TexCoords), mixRatio));
        diffuse = light.diffuse * diff * vec3(mix(texture(earth.diffuse, TexCoords), texture(grass.diffuse, TexCoords), mixRatio));
        specular = light.specular * spec * vec3(mix(texture(earth.specular, TexCoords), texture(grass.specular, TexCoords), mixRatio));
    }
    else{ //EARTH
        spec = pow(max(dot(viewDir, reflectDir), 0.0), earth.shininess);     
        ambient = light.ambient * vec3(texture(earth.diffuse, TexCoords));
        diffuse = light.diffuse * diff * vec3(texture(earth.diffuse, TexCoords));
        specular = light.specular * spec * vec3(texture(earth.specular, TexCoords));
    }
    // combine results

    return (ambient + diffuse + specular);
}