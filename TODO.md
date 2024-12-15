## THESE ARE QUESTIONS FOR THE COMMON SUPERMEGAJUNIOR GAME DEVELOPER


- DONE make healthbar for enemies
- fix rotation of player
- autofire
- better maps
- levels defined in json
- port frames file to json
- reestructure code
- make entity factory more generic
- destroy bullets when they are offscreen
- README
- support for windows, right now all is invaded by clang and macos stuff (LLD, bundling and frameworks) 
- should i stick to my_assets or better to more flexibility and add compatibility between specializations of assets<T>??
- STATE CLEARLY FOR SYSTEMS PURPOSES AND OWNERSHIP OF RESOURCES
- MORE TESTS FOR LEVEL AND LEVEL MANAGER
- SHOULD I USE POLYLINE FOR ENEMY PATH??
- sfml modules will be removed
- gui should also be a step
- WIP is asset Manager needed??
    - Maybe a 'keepalive' flag for passing ownership or a Res<T> wrapper 
- tmx render, use library or do it myself 
- detach bullet from player, reimagin api 
    1. remove from ellipse
    2. return the detached bullet
    3. add it to detached_bullets owned by physics system
    4. should have a chase component that has a start, midpoint and target (this changes)
        - target pos changes every frame and should be known by the physics system
