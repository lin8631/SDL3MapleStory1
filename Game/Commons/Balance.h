#pragma once

// Global balance multipliers to ease tuning of combat metrics
// Adjust DAMAGE_SCALE to decrease or increase overall damage
namespace Balance {
    // Example: 0.6 means damage is reduced to 60% of its base value
    constexpr float DAMAGE_SCALE = 0.6f;
}
