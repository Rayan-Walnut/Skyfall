# Skyfall

Skyfall — prototype minimal d'un battle royal (base).

But: Ce dépôt contient un squelette C++/OpenGL avec :
- Fenêtre OpenGL via GLFW + GLAD
- Caméra simple (third-person)
- Contrôles de déplacement (WASD + jump)
- Menu de démarrage avec bouton Start (bas droite)
- Tests unitaires simples

Build (Windows, PowerShell):

```powershell
mkdir build
Set-Location build
cmake .. -DCMAKE_BUILD_TYPE=Release
cmake --build . --config Release
```

Lancer l'exécutable `Skyfall` depuis le dossier `build`.

Note: CMake va récupérer GLFW/GLM/GLAD via FetchContent; une connexion Internet est requise la première configuration.
