# MovingAI Benchmark File Formats

## `.map` File Format

Example:
```
type octile
height 32
width 32
map
........................
................%.......
.......%...............%
.....@..................
........................
```

### Meaning:
- `type octile`: Movement type (e.g., 8-directional).
- `height 32`, `width 32`: Map dimensions.
- `map`: Start of the grid.
- `.`: Walkable terrain, `@`: Hard obstacle, `%`: Soft obstacle.

## `.scen` File Format

Example:
```
version 1
3	maze-32-32-2.map	32	32	17	21	15	16	13.82842712
```

### Meaning:
- `3`: Scenario ID.
- `maze-32-32-2.map`: Corresponding map file.
- `32 32`: Map dimensions.
- `17 21`: Start position (X, Y).
- `15 16`: Goal position (X, Y).
- `13.82842712`: Optimal path cost.

