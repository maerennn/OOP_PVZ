#ifndef GAME_CONFIG_HPP
#define GAME_CONFIG_HPP

#include <glm/glm.hpp>

/**
 * ============================================================================
 *  PvZ Grid Layout Design — PTSD Coordinate System
 * ============================================================================
 *
 *  PTSD window:  1280 × 720 px
 *  Origin (0,0): center of window
 *  X axis:       -640 (left)  → +640 (right)
 *  Y axis:       -360 (bottom)→ +360 (top)     ← Y goes UP
 *
 *  Original PvZ (800×600) has a 9-column × 5-row lawn grid.
 *  We scale up for 1280×720 while keeping roughly the same proportions.
 *
 *  Visual Layout (not to scale):
 *
 *     ┌─────────────────────────────────────────────────────┐  y = 360
 *     │                  SEED BANK / TOP BAR                │
 *     │               (y ∈ [260, 360] → 100 px)            │
 *     ├──────┬──────────────────────────────────┬───────────┤  y = 260
 *     │      │  LAWN GRID  9 × 5               │  ZOMBIE   │
 *     │ LAWN │  Cell: 80 × 100                  │  APPROACH │
 *     │MOWER │  Total: 720 × 500                │  ZONE     │
 *     │ AREA │  x ∈ [-340, 380]                 │  260 px   │
 *     │300px │  y ∈ [-240, 260]                 │           │
 *     ├──────┴──────────────────────────────────┴───────────┤  y = -240
 *     │               BOTTOM MARGIN (120 px)                │
 *     └─────────────────────────────────────────────────────┘  y = -360
 *    x=-640                                               x=+640
 *
 * ============================================================================
 */

namespace GameConfig {

// ── Window ──────────────────────────────────────────────────────────────────
constexpr int WINDOW_WIDTH  = 800;
constexpr int WINDOW_HEIGHT = 600;

// ── Grid Dimensions ─────────────────────────────────────────────────────────
constexpr int GRID_ROWS = 5;  // lanes (top = row 0)
constexpr int GRID_COLS = 9;  // columns (left = col 0)

// ── Cell Size (pixels) ─────────────────────────────────────────────────────
constexpr float CELL_WIDTH  = 80.0f *1.15f;
constexpr float CELL_HEIGHT = 100.0f *1.15f;

// ── Grid Origin ─────────────────────────────────────────────────────────────
//  "Origin" = the TOP-LEFT corner of cell (0, 0) in PTSD screen coordinates.
//  Since PTSD Y goes up, the top-left is the point with max Y and min X.
constexpr float GRID_ORIGIN_X = -269.0f ;   // left edge of column 0
constexpr float GRID_ORIGIN_Y =  241.0f ;   // top  edge of row 0

// ── Derived: Full Grid Extents ──────────────────────────────────────────────
constexpr float GRID_WIDTH  = GRID_COLS * CELL_WIDTH;   // 720
constexpr float GRID_HEIGHT = GRID_ROWS * CELL_HEIGHT;  // 500

constexpr float GRID_RIGHT  = GRID_ORIGIN_X + GRID_WIDTH;    //  380
constexpr float GRID_BOTTOM = GRID_ORIGIN_Y - GRID_HEIGHT;   // -240

// ── Layout Zones ────────────────────────────────────────────────────────────
//  Top bar (seed bank, sun counter)
constexpr float TOP_BAR_TOP    =  360.0f;
constexpr float TOP_BAR_BOTTOM =  GRID_ORIGIN_Y;             //  260
constexpr float TOP_BAR_HEIGHT =  TOP_BAR_TOP - TOP_BAR_BOTTOM; // 100

//  Lawn-mower column (left of grid)
constexpr float MOWER_ZONE_LEFT  = -640.0f;
constexpr float MOWER_ZONE_RIGHT = GRID_ORIGIN_X;            // -340
constexpr float MOWER_ZONE_WIDTH = MOWER_ZONE_RIGHT - MOWER_ZONE_LEFT; // 300

//  Zombie approach zone (right of grid)
constexpr float ZOMBIE_ZONE_LEFT  = GRID_RIGHT;              //  380
constexpr float ZOMBIE_ZONE_RIGHT = 640.0f;
constexpr float ZOMBIE_ZONE_WIDTH = ZOMBIE_ZONE_RIGHT - ZOMBIE_ZONE_LEFT; // 260

// ── Helper: Grid (row, col) → Screen Center Position ────────────────────────
/**
 * @brief  Convert a grid cell (row, col) to the PTSD screen-space position
 *         of that cell's CENTER.
 *
 * @param  row  Row index (0 = top lane,  4 = bottom lane)
 * @param  col  Column index (0 = leftmost, 8 = rightmost)
 * @return glm::vec2 with x, y in PTSD coordinates
 *
 * Example:
 *   Cell (0,0) center → (-300, 210)
 *   Cell (4,8) center → ( 340,-190)
 */
inline glm::vec2 CellToPosition(int row, int col) {
    return {
        GRID_ORIGIN_X + col * CELL_WIDTH  + CELL_WIDTH  * 0.5f,
        GRID_ORIGIN_Y - row * CELL_HEIGHT - CELL_HEIGHT * 0.5f
    };
}

// ── Helper: Screen Position → Grid (row, col)  ─────────────────────────────
/**
 * @brief  Convert a PTSD screen-space position to grid cell indices.
 *
 * @param  pos  Screen position (e.g. cursor position)
 * @param[out] row  Resulting row index
 * @param[out] col  Resulting column index
 * @return true if the position falls inside the grid, false otherwise
 */
inline bool PositionToCell(const glm::vec2 &pos, int &row, int &col) {
    float relX = pos.x - GRID_ORIGIN_X;         // offset from grid left
    float relY = GRID_ORIGIN_Y - pos.y;         // offset from grid top (flip Y)

    col = static_cast<int>(relX / CELL_WIDTH);
    row = static_cast<int>(relY / CELL_HEIGHT);

    return (col >= 0 && col < GRID_COLS &&
            row >= 0 && row < GRID_ROWS);
}

// ── Helper: Lane center Y from row index ────────────────────────────────────
/**
 * @brief  Get the Y coordinate of a lane's horizontal center line.
 * @param  row  Row/lane index (0–4)
 */
inline float LaneCenterY(int row) {
    return GRID_ORIGIN_Y - row * CELL_HEIGHT - CELL_HEIGHT * 0.5f;
}

// ── Gameplay Constants ──────────────────────────────────────────────────────
constexpr float ZOMBIE_SPAWN_X = ZOMBIE_ZONE_RIGHT;  // zombies appear at right edge
constexpr int STARTING_SUN = 999;  // Initial sun amount for player

// ── Seed Bank Layout ────────────────────────────────────────────────────────
namespace SeedBank {
    constexpr float BANK_X = -500.0f;           // Position from center
    constexpr float BANK_Y = 300.0f;            // Near top
    constexpr float PACKET_WIDTH = 50.0f;       // Width of each packet
    constexpr float PACKET_HEIGHT = 70.0f;      // Height of each packet
    constexpr float PACKET_START_X = -450.0f;   // First packet X position
    constexpr float PACKET_Y = 298.0f;          // Packets Y position
    constexpr float PACKET_SPACING = 53.0f;     // Space between packet centers
} // namespace SeedBank

// ── Z-Index Layers (higher = drawn on top) ──────────────────────────────────
namespace ZIndex {
    constexpr float BACKGROUND  =  0.0f;
    constexpr float GRID_LINES  =  1.0f;
    constexpr float LAWN_MOWER  =  2.0f;
    constexpr float PLANT       =  5.0f;
    constexpr float PROJECTILE  =  6.0f;
    constexpr float ZOMBIE      =  7.0f;
    constexpr float SUN         =  8.0f;
    constexpr float UI_OVERLAY  = 10.0f;
    constexpr float SEED_BANK   = 11.0f;
    constexpr float CURSOR_ITEM = 12.0f;
} // namespace ZIndex

} // namespace GameConfig

#endif // GAME_CONFIG_HPP

