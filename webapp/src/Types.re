/* Types.re holds types and transformations used throughout the UI components */

/* payload types to match types in /src/schema.rs */

/* type expected = Core | Extended | Unscheduled; */

/* Type declarations */
type day = {
  weekday: string,
  expected: string,
  actual: bool,
};

type kid = {
  name: string,
  schedule: day,
};

type classroom = {
  letter: string,
  capacity: int,
  collected: bool,
  kids: ref(array(kid)),
};

type school = {
  classrooms: array(classroom),
  weekday: string,
};

let get_uncollected_rooms = school =>
  /* Returns the letters of rooms havent been marked complete as a string */
  List.fold_left(
    (s, r) => s ++ r.letter ++ " ",
    "",
    List.filter(r => !r.collected, Array.to_list(school.classrooms)),
  );

let get_extended_letter_M8 = letter =>
  /* Summer '18 config */
  switch (letter) {
  | "A" => "AE"
  | "C" => "CE"
  | "B"
  | "D" => "DE"
  | "E"
  | "F"
  | "G" => "EE"
  | "I"
  | "J"
  | "K" => "JE"
  | "N"
  | "O" => "NE"
  | _ => "ERR!!!"
  };

let get_extended_letter_F8 = letter =>
  /* Fall '18 config */
  switch (letter) {
  | "A"
  | "C" => "CE"
  | "B"
  | "D" => "DE"
  | "E"
  | "F"
  | "G" => "EE"
  | "J"
  | "K"
  | "H"
  | "I" => "IE"
  | "L"
  | "M"
  | "N"
  | "O" => "ME"
  | _ => "ERR!!!"
  };

let get_extended_kids = (school, extended_config) =>
  /* Returns a school with only the Extended Day kids */
  {
    ...school,
    classrooms:
      Array.map(
        r => {
          ...r,
          letter:
            switch (extended_config) {
            | "M8" => get_extended_letter_M8(r.letter)
            | "F8" => get_extended_letter_F8(r.letter)
            },
          kids:
            ref(
              Array.of_list(
                List.filter(
                  k => k.schedule.expected == "Extended",
                  Array.to_list(r.kids^),
                ),
              ),
            ),
        },
        school.classrooms,
      ),
  };

let add_extended_room = (school, classroom) => {
  /* This is our folding fn from get_extended_rooms below.
     It should take a room and a school and either add the new room
     or if a room already exists with the same letter, just add those kids */
  let target = ref(school.classrooms);

  if (Array.length(target^) == 0) {
    target := Array.append(target^, Array.make(1, classroom));
  } else {
    let already_included = Array.map(oldr => oldr.letter, school.classrooms);
    let found = ref(false);
    let idx = ref(0) /* This will only be read later if found is toggled to true*/;
    Array.iteri(
      (i, l) =>
        if (classroom.letter == l) {
          found := true;
          idx := i;
        },
      already_included,
    );
    if (found^) {
      /* We've already seen this letter - mash the new kid list into the matching existing kid list */
      let old_classroom = school.classrooms[idx^];
      let new_classroom = {
        ...classroom,
        kids: ref(Array.append(old_classroom.kids^, classroom.kids^)),
      };
      target^[idx^] = new_classroom;
    } else {
      /* This is a new extended day room - add it as-is */
      target := Array.append(target^, Array.make(1, classroom));
    };
  };

  {...school, classrooms: target^};
};

let get_extended_rooms = (school, extended_config) => {
  /* Returns a `school` of the extended kids */
  let s = get_extended_kids(school, extended_config);
  Array.fold_left(
    add_extended_room,
    {...school, classrooms: [||]},
    s.classrooms,
  );
};

let toggle = (school, kid) => {
  /* Returns a new school with the specified kid toggled */
  ...school,
  classrooms:
    Array.map(
      room => {
        ...room,
        kids:
          ref(
            Array.map(
              k =>
                if (kid == k) {
                  {
                    ...kid,
                    schedule: {
                      ...kid.schedule,
                      actual: !kid.schedule.actual,
                    },
                  };
                } else {
                  k;
                },
              room.kids^,
            ),
          ),
      },
      school.classrooms,
    ),
};

let toggle_collected = (school, classroom) => {
  /* Returns a new school with the specified classroom toggled */
  ...school,
  classrooms:
    Array.map(
      room =>
        if (classroom == room) {
          {...room, collected: !room.collected};
        } else {
          room;
        },
      school.classrooms,
    ),
};

[@bs.val] external alert: string => unit = "alert";
[@bs.val] external btoa: string => string = "btoa";
