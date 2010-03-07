#ifdef _MSC_VER
#include "stdint.h"
#else
#include <stdint.h>
#endif
#include <stddef.h>

enum wav2prg_plugin_endianness {
  lsbf,
  msbf
};

enum wav2prg_checksum {
  wav2prg_no_checksum,
  wav2prg_xor_checksum,
  wav2prg_add_checksum
};

struct wav2prg_tolerance {
  uint16_t less_than_ideal;
  uint16_t more_than_ideal;
};

enum wav2prg_tolerance_type {
  wav2prg_tolerant,
  wav2prg_adaptively_tolerant,
  wav2prg_intolerant
};

enum wav2prg_return_values {
  wav2prg_ok,
  wav2prg_invalid
};

enum wav2prg_findpilot_type {
  wav2prg_synconbit,
  wav2prg_synconbyte
};

enum wav2prg_sync_return_values {
  wav2prg_notsynced,
  wav2prg_synced,
  wav2prg_synced_and_one_byte_got
};

struct wav2prg_block {
  uint16_t start;
  uint16_t end;
  char name[17];
  unsigned char data[65536];
};

enum wav2prg_checksum_state {
  wav2prg_checksum_state_unverified,
  wav2prg_checksum_state_correct,
  wav2prg_checksum_state_load_error
};

struct wav2prg_context;
struct wav2prg_functions;
struct wav2prg_plugin_conf;
struct wav2prg_plugin_functions;

typedef enum wav2prg_return_values (*wav2prg_get_rawpulse_func)(void* audiotap, uint32_t* rawpulse);
typedef uint8_t                    (*wav2prg_test_eof_func)(void* audiotap);
typedef int32_t                    (*wav2prg_get_pos_func)(void* audiotap);
typedef enum wav2prg_return_values (*wav2prg_get_pulse_func)(struct wav2prg_context*, const struct wav2prg_functions*, struct wav2prg_plugin_conf*, uint8_t*);
typedef enum wav2prg_return_values (*wav2prg_get_bit_func)(struct wav2prg_context*, const struct wav2prg_functions*, struct wav2prg_plugin_conf*, uint8_t*);
typedef enum wav2prg_return_values (*wav2prg_get_byte_func)(struct wav2prg_context*, const struct wav2prg_functions*, struct wav2prg_plugin_conf*, uint8_t*);
typedef enum wav2prg_return_values (*wav2prg_get_word_func)(struct wav2prg_context*, const struct wav2prg_functions*, struct wav2prg_plugin_conf*, uint16_t*);
typedef enum wav2prg_return_values (*wav2prg_get_word_bigendian_func)(struct wav2prg_context*, const struct wav2prg_functions*, struct wav2prg_plugin_conf*, uint16_t*);
typedef enum wav2prg_return_values (*wav2prg_get_block_func)(struct wav2prg_context*, const struct wav2prg_functions*, struct wav2prg_plugin_conf*, uint8_t*, uint16_t*, uint16_t*);
typedef enum wav2prg_return_values (*wav2prg_get_sync)(struct wav2prg_context*, const struct wav2prg_functions*, struct wav2prg_plugin_conf*);
typedef enum wav2prg_sync_return_values (*wav2prg_get_first_sync)(struct wav2prg_context*, const struct wav2prg_functions*, struct wav2prg_plugin_conf*, uint8_t*);
typedef enum wav2prg_return_values (*wav2prg_get_block_info)(struct wav2prg_context*, const struct wav2prg_functions*, struct wav2prg_plugin_conf*, char*, uint16_t*, uint16_t*);
typedef enum wav2prg_checksum_state (*wav2prg_check_checksum)(struct wav2prg_context*, const struct wav2prg_functions*, struct wav2prg_plugin_conf*);
typedef enum wav2prg_return_values (*wav2prg_get_loaded_checksum)(struct wav2prg_context*, const struct wav2prg_functions*, struct wav2prg_plugin_conf*, uint8_t*);
typedef void                       (*wav2prg_update_checksum)(struct wav2prg_context*, uint8_t);
typedef uint8_t                    (*wav2prg_compute_checksum_step)(struct wav2prg_plugin_conf*, uint8_t, uint8_t);
typedef void                       (*wav2prg_enable_checksum)(struct wav2prg_context*);
typedef void                       (*wav2prg_disable_checksum)(struct wav2prg_context*);
typedef const struct wav2prg_plugin_conf* (*wav2prg_get_new_plugin_state)(void);
typedef void                       (*wav2prg_register_loader)(const struct wav2prg_plugin_functions* functions, const char* name);
typedef uint8_t                    (*wav2prg_recognize_block_as_mine)(struct wav2prg_plugin_conf*, uint8_t*, uint16_t, uint16_t);
typedef uint8_t                    (*wav2prg_recognize_block_as_mine_with_start_end)(struct wav2prg_plugin_conf*, uint8_t*, uint16_t, uint16_t, char*, uint16_t*, uint16_t*);

struct wav2prg_functions {
  wav2prg_get_sync get_sync;
  wav2prg_get_pulse_func get_pulse_func;
  wav2prg_get_bit_func get_bit_func;
  wav2prg_get_byte_func get_byte_func;
  wav2prg_get_word_func get_word_func;
  wav2prg_get_word_bigendian_func get_word_bigendian_func;
  wav2prg_get_block_func get_block_func;
  wav2prg_check_checksum check_checksum_func;
  wav2prg_get_loaded_checksum get_loaded_checksum_func;
  wav2prg_enable_checksum enable_checksum_func;
  wav2prg_disable_checksum disable_checksum_func;
};

struct wav2prg_generate_private_state 
{
  uint32_t size;
  const void* model;
};

struct wav2prg_plugin_functions {
  wav2prg_get_bit_func get_bit_func;
  wav2prg_get_byte_func get_byte_func;
  wav2prg_get_first_sync get_first_sync;
  wav2prg_get_block_info get_block_info;
  wav2prg_get_block_func get_block_func;
  wav2prg_get_new_plugin_state get_new_plugin_state;
  wav2prg_compute_checksum_step compute_checksum_step;
  wav2prg_get_loaded_checksum get_loaded_checksum_func;
  wav2prg_recognize_block_as_mine recognize_block_as_mine_func;
  wav2prg_recognize_block_as_mine_with_start_end recognize_block_as_mine_with_start_end_func;
};

enum wav2prg_kernal_dependency {
  wav2prg_kernal_header,
  wav2prg_kernal_data
};

struct wav2prg_dependency {
  enum wav2prg_kernal_dependency kernal_dependency;
  const char *other_dependency;
  uint8_t dependency_is_recommended;
};

struct wav2prg_plugin_conf {
  enum wav2prg_plugin_endianness endianness;
  enum wav2prg_checksum checksum_type;
  uint8_t num_pulse_lengths;
  uint16_t *thresholds;
  uint16_t *ideal_pulse_lengths;
  enum wav2prg_findpilot_type findpilot_type;
  uint8_t pilot_byte;
  uint8_t len_of_pilot_sequence;
  uint8_t *pilot_sequence;
  struct wav2prg_dependency* dependency;
  void* private_state;
};

struct plugin_tree {
  const char* node;
  struct plugin_tree* first_child;
  struct plugin_tree* first_sibling;
};

struct wav2prg_context;

void wav2prg_get_new_context(wav2prg_get_rawpulse_func rawpulse_func,
                             wav2prg_test_eof_func test_eof_func,
                             wav2prg_get_pos_func get_pos_func,
                             enum wav2prg_tolerance_type tolerance_type,
                             const char* loader_name,
                             const char** loader_names,
                             struct wav2prg_tolerance* tolerances,
                             void* audiotap);
#if 0 //defined _WIN32
#elif defined DSDS
#else
#define PLUGIN_ENTRY(x) \
  void x##_get_plugin(wav2prg_register_loader register_loader_func)
#endif

