# a fluid simulation

splish, splash welcome to the world of fluid simulations where the entire goal is to make pixels go splish splash for amusment. More serously the goal of this project is to fellow [this paper](https://matthias-research.github.io/pages/publications/sca03.pdf) for creating a little cool fluid simulation.

## fluid rendering

The fluid rendering renders the particles as tiny circles using the GPU pipline. I send the particles positions then the GPU renders them as circles (yep and that is 200 lines of code !). I used GLFW with OpenGL for rendering.
