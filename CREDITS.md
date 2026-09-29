# Credits and asset provenance

## Team implementation

Zhina Bagheri, Yashar, Sepehr Salehi, and Mahdi Roustaei. Developed for Computergrafik 1, Hochschule Hannover, summer semester 2026. Contributions are recorded in the README using the original team's attribution.

## Backgammon model

- Files: `assets/models/backgammon.obj` and `backgammon.mtl`.
- Title: **Backgammon**.
- Creator: **Requital (Azamkhon)**.
- Source: [Backgammon on Sketchfab](https://sketchfab.com/3d-models/none-9532543f8ca74bec800506261b47d6d4).
- License: [Creative Commons Attribution 4.0 International](https://creativecommons.org/licenses/by/4.0/).
- Changes documented by the original team: prepared in Blender, triangulated faces, and recalculated normals. The application uses its own wood texture and material setup.
- Verification: Sketchfab's public model API identifies this title and creator and supplies the CC BY 4.0 license URL. Attribution matches the original README; the exported Blender mesh was not compared byte-for-byte with a downloaded original model.

## Environment cubemap

- Files: `assets/skybox/posx.jpg`, `negx.jpg`, `posy.jpg`, `negy.jpg`, `posz.jpg`, and `negz.jpg`.
- Title: **Yokohama 2**.
- Creator: **Emil Persson, also known as Humus**.
- Source: [Humus texture collection](https://www.humus.name/index.php?page=Textures), [asset permalink](https://www.humus.name/index.php?page=Textures&ID=138).
- License: [Creative Commons Attribution 3.0 Unported](https://creativecommons.org/licenses/by/3.0/).
- Changes: none to the six bundled JPG files. Each file's SHA-256 matches the corresponding file in the author's `Yokohama2.zip` download.

## Surface textures

| File | Asset / creator | Source | License |
| --- | --- | --- | --- |
| `assets/textures/wood.jpg` | Wood Floor Worn / Poly Haven | [Asset page](https://polyhaven.com/a/wood_floor_worn) | [CC0](https://polyhaven.com/license) |
| `assets/textures/grass.jpg` | Aerial Grass Rock / Poly Haven | [Asset page](https://polyhaven.com/a/aerial_grass_rock) | [CC0](https://polyhaven.com/license) |

These two asset identifications are retained from the original README. Poly Haven's source pages and CC0 policy were checked; the bundled texture files were not independently matched against every available source resolution or conversion.

The original README attributes the cube, floor, wall, pyramid, glass-panel geometry and the checker, floor-grid, and brick PPM textures to the team.

## Libraries

- [GLFW](https://www.glfw.org/) — window creation and input; installed externally.
- [GLEW](https://glew.sourceforge.net/) — OpenGL function loading; installed externally.
- [stb_image](https://github.com/nothings/stb) — bundled version 2.30 in `src/stb_image.h`; its original MIT / public-domain dual-license notice remains intact in that file.

## Screenshot and portfolio preparation

`docs/scene.png` is a direct capture of the application's OpenGL framebuffer using the credited model and textures. It was captured in a temporary hidden test context with a fixed animation time and a downward camera angle; rendering source and shader behavior were not changed. The image is not an AI-generated mockup.

This portfolio copy adds documentation, resolved asset credits, the rendered preview, expanded ignore rules, and a platform-aware build file. Application source, shaders, and bundled assets are unchanged from the supplied archive.

No project-wide license for the team's code was specified in the source archive. These credits do not grant a new license on behalf of the team, and third-party asset licenses are not replaced by any future code license.
