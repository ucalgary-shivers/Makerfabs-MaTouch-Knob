#ifndef __LIBMAPPER_STATE_H__
#define __LIBMAPPER_STATE_H__

inline xQueueHandle libmmaper_state_queue_;
inline SemaphoreHandle_t libmapper_mutex;

struct LibmapperState {
  bool isLibmapperMode;
  float torque;
  float anglePosition;
  bool network_ready;
};

enum class LibmapperField {
    IsLibmapperMode,
    torque,
    anglePosition,
    network_ready
};


void publish_libmapper(const LibmapperState& state);

void update_libmapper(LibmapperField field, bool value);

void update_libmapper(LibmapperField field, float value);

inline void publish_libmapper(const LibmapperState& state) {
  xQueueOverwrite(libmmaper_state_queue_, &state);
}

inline void update_libmapper_bool(LibmapperField field, bool value) {
    if (xSemaphoreTake(libmapper_mutex, pdMS_TO_TICKS(10)) == pdTRUE) {
        LibmapperState state;
        if (xQueuePeek(libmmaper_state_queue_, &state, 0) == pdFALSE)
        {
            state = { false, 0.0f, 0.0f, false };
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
            state = { false, 0.0f, 0.0f, false };
        }

        switch (field) {
            case LibmapperField::torque:
                state.torque = value;
                break;

            case LibmapperField::anglePosition:
                state.anglePosition = value;
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