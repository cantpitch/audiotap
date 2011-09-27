#include "observers.h"
#include "wav2prg_api.h"

#include <stdlib.h>
#include <string.h>

static struct wav2prg_observed {
  const char *name;
  struct wav2prg_observed_loaders *observers;
} *observed_list = NULL;

static void add_observer_to_list(struct wav2prg_observed_loaders **observers, const char* observer_name, wav2prg_recognize_block recognize_func){
  struct wav2prg_observed_loaders *current_observers;
  int number_of_observers = 0;

  for(current_observers = *observers; current_observers->loader != NULL; current_observers++, number_of_observers++);
  *observers = realloc(*observers, sizeof(**observers) * (2 + number_of_observers));
  (*observers)[number_of_observers].loader = observer_name;
  (*observers)[number_of_observers].recognize_func = recognize_func;
  (*observers)[number_of_observers + 1].loader = NULL;
  (*observers)[number_of_observers + 1].recognize_func = NULL;
}

static struct wav2prg_observed_loaders** get_list_of_observers_maybe_adding_observed(const char *observed_name, enum wav2prg_bool add_if_missing){
  struct wav2prg_observed *current_observed;
  int number_of_observed = 0;

  if (observed_list == NULL)
  {
    observed_list = calloc(sizeof(struct wav2prg_observed), 1);
  }
  for(current_observed = observed_list; current_observed->name != NULL; current_observed++, number_of_observed++){
    if (!strcmp(observed_name, current_observed->name))
      return &current_observed->observers;
  }
  if (add_if_missing)
  {
    observed_list = realloc(observed_list, sizeof(observed_list[0]) * (2 + number_of_observed));
    current_observed = &observed_list[number_of_observed];
    current_observed->name = observed_name;
    current_observed->observers = calloc(sizeof(struct wav2prg_observed_loaders), 1);
    current_observed[1].name = NULL;
    current_observed[1].observers = NULL;
    return &current_observed->observers;
  }
  return NULL;
}

static void add_one_observed(const char *observer_name, const char *observed_name, wav2prg_recognize_block recognize_func){
  if (!strcmp(observed_name, "khc"))
  {
    add_one_observed(observer_name, "Kernal header chunk 1st copy", recognize_func);
    add_one_observed(observer_name, "Kernal header chunk 2nd copy", recognize_func);
  }
  else if (!strcmp(observed_name, "kdc"))
  {
    add_one_observed(observer_name, "Kernal data chunk 1st copy", recognize_func);
    add_one_observed(observer_name, "Kernal data chunk 2nd copy", recognize_func);
  }
  else
    add_observer_to_list(get_list_of_observers_maybe_adding_observed(observed_name, wav2prg_true), observer_name, recognize_func);
}

void add_observed(const char *observer_name, const struct wav2prg_observed_loaders *observed){
  const struct wav2prg_observed_loaders *current_observed;

  for(current_observed = observed; current_observed->loader != NULL; current_observed++)
    add_one_observed(observer_name, current_observed->loader, current_observed->recognize_func);
}

struct wav2prg_observed_loaders* get_observers(const char *observed_name){
  struct wav2prg_observed_loaders **observer = get_list_of_observers_maybe_adding_observed(observed_name, wav2prg_false);
  return observer ? *observer : NULL;
}
