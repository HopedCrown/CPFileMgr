#ifndef BATTERY_H
#define BATTERY_H

// ============================================================================
// LOWERCASE CONSTANTS & ENUMS
// ============================================================================
enum battery_type { 
    alkaline, 
    nimh, 
    dead 
};

// ============================================================================
// OS TYPEDEFS (os_func_name_t FORMAT)
// ============================================================================
typedef int (*os_Battery_GetVoltage_t)(void);
typedef void* (*os_LCD_GetVRAMAddress_t)(void);
typedef void (*os_LCD_Refresh_t)(void);
typedef void (*os_LCD_VRAMBackup_t)(void);
typedef void (*os_LCD_VRAMRestore_t)(void);

// ============================================================================
// INLINE FUNCTION DEFINITIONS
// ============================================================================

/**
 * Calculates battery percentage using non-linear piecewise tracking.
 * Takes the battery_type enum to select the calculation path.
 *
 * @param measured_cv Raw voltage in centivolts (cV) from Battery_GetVoltage()
 * @param chemistry Active battery_type enum (alkaline, nimh, or dead)
 * @return Remainder percentage bound strictly from 0 to 100
 */
inline int Battery_GetPercent(int measured_cv,enum battery_type chemistry) {
    // Safety Catch: If the battery is flagged as dead, always return 0%
    if (chemistry == dead || measured_cv < 100) {
        return 0;
    }

    if (chemistry == alkaline) {
        if (measured_cv >= 155) return 100;
        if (measured_cv >= 140) return 80 + ((measured_cv - 140) * 20) / 15;
        if (measured_cv >= 128) return 60 + ((measured_cv - 128) * 20) / 12;
        if (measured_cv >= 120) return 40 + ((measured_cv - 120) * 20) / 8;
        if (measured_cv >= 115) return 20 + ((measured_cv - 115) * 20) / 5;
        return ((measured_cv - 100) * 20) / 15; // Range: 100 cV to 115 cV
    }

    if (chemistry == nimh) {
        if (measured_cv >= 140) return 100;
        if (measured_cv >= 130) return 80 + ((measured_cv - 130) * 20) / 10;
        if (measured_cv >= 125) return 60 + ((measured_cv - 125) * 20) / 5;
        if (measured_cv >= 120) return 40 + ((measured_cv - 120) * 20) / 5;
        if (measured_cv >= 115) return 20 + ((measured_cv - 115) * 20) / 5;
        return ((measured_cv - 100) * 20) / 15; // Range: 100 cV to 115 cV
    }

    return 0;
}

/**
 * Runs a rapid synchronous VRAM stress test to read internal resistance signatures.
 * Safely exits immediately if the cell is already under 1.0V.
 *
 * @return Detected dynamic state (alkaline, nimh, or dead)
 */
inline enum battery_type Battery_GetChemistry() {
    // Function pointer initialization mapping directly to your symbol addresses
    os_Battery_GetVoltage_t Battery_GetVoltage = (os_Battery_GetVoltage_t)0x8002a614;
    os_LCD_GetVRAMAddress_t LCD_GetVRAMAddress = (os_LCD_GetVRAMAddress_t)0x8002e13c;
    os_LCD_Refresh_t        LCD_Refresh        = (os_LCD_Refresh_t)0x800373ac;
    os_LCD_VRAMBackup_t     LCD_VRAMBackup     = (os_LCD_VRAMBackup_t)0x8002d3e2;
    os_LCD_VRAMRestore_t    LCD_VRAMRestore    = (os_LCD_VRAMRestore_t)0x8002d402;

    int v_baseline = Battery_GetVoltage();

    // MANDATORY GATE: If the battery is under 1.0V (100 cV), it's dead.
    // Return dead directly and skip the stress test to avoid a hard system crash.
    if (v_baseline < 100) {
        return dead; 
    }

    // Backup current screen content invisibly on the main execution path
    LCD_VRAMBackup();

    // Flood VRAM with maximum power white pixels (320x528 screen area grid)
    unsigned short* vram = (unsigned short*)LCD_GetVRAMAddress();
    if (vram != (unsigned short*)0) {
        int total_pixels = 320 * 528;
        for (int i = 0; i < total_pixels; ++i) {
            vram[i] = 0xFFFF; 
        }
        
        // Push maximum current configuration directly to LCD panel hardware
        LCD_Refresh(); 
        
        // Critical: Force a tiny delay loop to allow the hardware rails to sag 
        // before sampling. 'volatile' ensures compiler doesn't optimize it away.
        for (volatile int delay = 0; delay < 40000; ++delay) {
            // Wait for analog voltage settling time
        }
    }

    // Capture the immediate voltage drop reading under load
    int v_load = Battery_GetVoltage();

    // Instantly restore user screen frame buffer 
    LCD_VRAMRestore();
    LCD_Refresh();

    int v_delta = v_baseline - v_load;

    // A drop greater than 5 cV indicates high internal resistance (alkaline)
    if (v_delta > 5) {
        return alkaline;
    }
    
    return nimh;
}

#endif // BATTERY_H
