# UNCC Quadrotor #

## Structure ##

The repository is divided into the following:
- *Make:* By running the make file, library paths will be made known to the compilers.
- *Swarm* and *VisionSystem*: Libraries to the major components of the quadrotor project

The folder structure is as follows:
- UNCCQuadrotor (root)
  - Make
  - Swarm
    - Implementations
    - Includes
    - SourceCode
    - Tests
  - VisionSystem
    - Implementations
    - Includes
    - SourceCode
    - Tests

## Organization of *Swarm* and *VisionSystem* ##
Each of these folders have the following:
- Implementations: Assemblies that implement files from *SourceCode*
- Includes: All relevant libraries for the respective component of the project
- SourceCode: 'Nuf said
- Tests: Code that has been altered or created for the purpose of testing the system's behavior


