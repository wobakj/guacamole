#define WARP_MODE @gbuffer_warp_mode@

#define WARP_MODE_NONE                                0
#define WARP_MODE_POINTS                              1
#define WARP_MODE_SCALED_POINTS                       2
#define WARP_MODE_QUADS_SCREEN_ALIGNED                3
#define WARP_MODE_QUADS_NORMAL_ALIGNED                4
#define WARP_MODE_QUADS_DEPTH_ALIGNED                 5
#define WARP_MODE_GRID_DEPTH_THRESHOLD                6
#define WARP_MODE_GRID_SURFACE_ESTIMATION             7
#define WARP_MODE_GRID_SURFACE_ESTIMATION_STRETCH     8
#define WARP_MODE_GRID_ADVANCED_SURFACE_ESTIMATION    9
#define WARP_MODE_GRID_NON_UNIFORM_SURFACE_ESTIMATION 10

#define INTERPOLATION_MODE @interpolation_mode@
#define INTERPOLATION_MODE_NEAREST                    0
#define INTERPOLATION_MODE_LINEAR                     1
#define INTERPOLATION_MODE_ADAPTIVE                   2
