use std::collections::HashMap;
use regex::Regex;

fn part1_is_passport_good(passport: &HashMap<&str, &str>) -> bool {
    passport.len() == 8 || (passport.len() == 7 && !passport.contains_key("cid"))
}

fn is_birth_year_valid(year: &str) -> bool {
    match year.parse::<i32>() {
        Ok(v) => v >= 1920 && v <= 2002,
        Err(_) => false
    }
}

fn is_issue_year_valid(year: &str) -> bool {
    match year.parse::<i32>() {
        Ok(v) => v >= 2010 && v <= 2020,
        Err(_) => false
    }
}

fn is_expiration_year_valid(year: &str) -> bool {
    match year.parse::<i32>() {
        Ok(v) => v >= 2020 && v <= 2030,
        Err(_) => false
    }
}

fn is_height_valid(height: &str) -> bool {
    let re_cm = Regex::new("^1([5-8][0-9]|9[0-3])cm$").unwrap();
    let re_in = Regex::new("^(59|6[0-9]|7[0-6])in$").unwrap();

    return re_cm.is_match(height) || re_in.is_match(height);
}

fn is_hair_color_valid(hair_color: &str) -> bool {
    let re_hair = Regex::new("^#[0-9a-f]{6}$").unwrap();
    return re_hair.is_match(hair_color);
}

fn is_eye_color_valid(eye_color: &str) -> bool {
    match eye_color {
        "amb" => true,
        "blu" => true,
        "brn" => true,
        "gry" => true,
        "grn" => true,
        "hzl" => true,
        "oth" => true,
        _ => false
    }
}

fn is_passport_id_valid(pid: &str) -> bool {
    return pid.len () == 9 &&
        match pid.parse::<i32>() {
            Ok(_) => true,
            Err(_) => false
        };
}

fn is_passport_record_good(key: &str, value: &str) -> bool {
    return match key {
        "byr" => is_birth_year_valid(value),
        "iyr" => is_issue_year_valid(value),
        "eyr" => is_expiration_year_valid(value),
        "hgt" => is_height_valid(value),
        "hcl" => is_hair_color_valid(value),
        "ecl" => is_eye_color_valid(value),
        "pid" => is_passport_id_valid(value),
        "cid" => true,
        _ => false
    }
}

fn part2_is_passport_good(passport: &HashMap<&str, &str>) -> bool {
    return part1_is_passport_good(passport) && passport.iter().all(|(k, v)| is_passport_record_good(k, v));
}

fn main() {
    let args: Vec<String> = std::env::args().collect();
    let filename = &args[1];
    let contents = std::fs::read_to_string(filename).expect("File not found");

    let blobs = contents.trim().split("\n\n");

    let mut passports : Vec<HashMap<&str, &str>> = Vec::new();

    for a in blobs {
        let mut passport = HashMap::new ();

        let records = a.trim().split_whitespace();

        for r in records {
            let mut key_value = r.split(':');
            passport.insert(key_value.next().expect("Key not found"),
                            key_value.next().expect("Value not found"));
        }

        passports.push(passport);
    }

    println!("Length of passports: {}", passports.len());
    println!("Good passports: {}", passports.iter().filter(|x| part2_is_passport_good(&x)).count());
}


#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn identity() {
        assert!(true);
    }

    #[test]
    fn heights() {
        assert!(is_height_valid("150cm"));
        assert!(is_height_valid("193cm"));
        assert!(is_height_valid("165cm"));
        assert!(is_height_valid("178cm"));
        assert!(is_height_valid("59in"));
        assert!(is_height_valid("76in"));
        assert!(is_height_valid("65in"));

        assert!(!is_height_valid("30cm"));
        assert!(!is_height_valid("123"));
        assert!(!is_height_valid("1029in"));
        assert!(!is_height_valid("029in"));
        assert!(!is_height_valid("2029cm"));
    }
}
