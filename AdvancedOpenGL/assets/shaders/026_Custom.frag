#version 450 core

in vec4 vs_color;

out vec4 color;

void main()
{
    // Calcule la position du fragment par rapport au centre de l'écran
    vec2 fragPos = gl_FragCoord.xy / vec2(800.0, 600.0); // Remplacez 800 et 600 par les dimensions de votre écran

    // Utilise la position du fragment pour créer un dégradé
    color = vec4(fragPos.x, fragPos.y, 0.5, 1.0);
}
