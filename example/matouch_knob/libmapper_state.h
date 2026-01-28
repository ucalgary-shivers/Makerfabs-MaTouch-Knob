#ifndef __LIBMAPPER_STATE_H__
#define __LIBMAPPER_STATE_H__

inline xQueueHandle libmmaper_state_queue_;
inline SemaphoreHandle_t libmapper_mutex;

struct LibmapperState {
    bool isLibmapperMode;
    float torque;
    float anglePosition;
    bool network_ready;  
    int32_t num_positions;
    int32_t position;
    float position_width_radians;
    float detent_strength_unit;
    float endstop_strength_unit;
    float snap_point;
};

enum class LibmapperField {
    IsLibmapperMode,
    torque,
    anglePosition,
    network_ready,
    num_positions,
    position,
    position_width_radians,
    detent_strength_unit,
    endstop_strength_unit,
    snap_point
};


inline void publish_libmapper(const LibmapperState& state) {
  xQueueOverwrite(libmmaper_state_queue_, &state);
}

inline void update_libmapper_bool(LibmapperField field, bool value) {
    if (xSemaphoreTake(libmapper_mutex, pdMS_TO_TICKS(10)) == pdTRUE) {
        LibmapperState state;
        if (xQueuePeek(libmmaper_state_queue_, &state, 0) == pdFALSE)
        {
            state = { false, 0.0f, 0.0f, false, 0, 0, 0.0f, 0.0f, 0.0f, 0.0f };
        }

        switch (field) {
            case LibmapperField::IsLibmapperMode:
                state.isLibmapperMode = value;
                break;

            case LibmapperField::network_ready:
                state.network_ready = value;
                break;

            default:
                // ignore: wrong type for this field
                break;
        }
        
        publish_libmapper(state);    
        xSemaphoreGive(libmapper_mutex);
    }
}

inline void update_libmapper_float(LibmapperField field, float value) {
    if (xSemaphoreTake(libmapper_mutex, pdMS_TO_TICKS(10)) == pdTRUE) {
        LibmapperState state;

        if (xQueuePeek(libmmaper_state_queue_, &state, 0) == pdFALSE)
        {
            state = { false, 0.0f, 0.0f, false, 0, 0, 0.0f, 0.0f, 0.0f, 0.0f };
        }

        switch (field) {
            case LibmapperField::torque:
                state.torque = value;
                break;

            case LibmapperField::anglePosition:
                state.anglePosition = value;
                break;

            case LibmapperField::position_width_radians:
                state.position_width_radians = value;
                break;

            case LibmapperField::detent_strength_unit:
                state.detent_strength_unit = value;
                break;

            case LibmapperField::endstop_strength_unit:
                state.endstop_strength_unit = value;
                break;

            case LibmapperField::snap_point:
                state.snap_point = value;
                break;

            default:
                // ignore: wrong type for this field
                break;
        }   
            
        publish_libmapper(state);    
        xSemaphoreGive(libmapper_mutex);
    }
}

inline void update_libmapper_int(LibmapperField field, int value) {
    if (xSemaphoreTake(libmapper_mutex, pdMS_TO_TICKS(10)) == pdTRUE) {
        LibmapperState state;

        if (xQueuePeek(libmmaper_state_queue_, &state, 0) == pdFALSE)
        {
            state = { false, 0.0f, 0.0f, false, 0, 0, 0.0f, 0.0f, 0.0f, 0.0f };
        }

        switch (field) {
            case LibmapperField::num_positions:
                state.num_positions = value;
                break;

            case LibmapperField::position:
                state.position = value;
                break;

            default:
                // ignore: wrong type for this field
                break;
        }   
            
        publish_libmapper(state);    
        xSemaphoreGive(libmapper_mutex);
    }
}

#endif