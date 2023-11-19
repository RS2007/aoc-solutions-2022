use std::{
    cmp::{max, min},
    collections::{BTreeMap, HashSet},
    fs,
};

use nom::{
    bytes::complete::tag,
    character::complete::digit1,
    combinator::{map_res, opt, recognize},
    error::Error,
    sequence::preceded,
    IResult,
};

#[derive(Debug)]
struct Beacon {
    x: i32,
    y: i32,
}

#[derive(Debug)]
struct Sensor {
    x: i32,
    y: i32,
}

fn parse_integer(input: &str) -> IResult<&str, isize> {
    map_res(recognize(preceded(opt(tag("-")), digit1)), |s| {
        isize::from_str_radix(s, 10)
    })(input)
}

fn get_allowed_x_for_y(coordinate_pairs: &Vec<(Beacon, Sensor)>, row: i32) -> Vec<i32> {
    let hash_set = coordinate_pairs
        .iter()
        .filter(|(beacon, sensor)| {
            let (beacon_x, beacon_y, sensor_x, sensor_y) = (beacon.x, beacon.y, sensor.x, sensor.y);
            let manhattan_distance = (beacon_y - sensor_y).abs() + (sensor_x - beacon_x).abs();
            let max_y_possible = manhattan_distance + sensor_y;
            let min_y_possible = -manhattan_distance + sensor_y;
            row >= min_y_possible && row <= max_y_possible
        })
        .flat_map(|(beacon, sensor)| {
            let (beacon_x, beacon_y, sensor_x, sensor_y) = (beacon.x, beacon.y, sensor.x, sensor.y);
            let manhattan_distance = (beacon_y - sensor_y).abs() + (sensor_x - beacon_x).abs();
            let indx1 = sensor_x + manhattan_distance - (row - sensor_y).abs();
            let indx2 = sensor_x - (manhattan_distance - (row - sensor_y).abs());
            min(indx1, indx2)..=max(indx1, indx2)
        })
        .collect::<HashSet<i32>>();
    hash_set
        .iter()
        .filter(|x| {
            coordinate_pairs
                .iter()
                .find(|(beacon, sensor)| {
                    (beacon.x == **x && beacon.y == row) || (sensor.x == **x && sensor.y == row)
                })
                .is_none()
        })
        .map(|iter| (*iter))
        .collect::<Vec<i32>>()
}

fn part1(input: &str, row: i32) -> i32 {
    let coordinate_pairs = parse(input);
    get_allowed_x_for_y(&coordinate_pairs, row).len() as i32
}

fn parse(input: &str) -> Vec<(Beacon, Sensor)> {
    input
        .lines()
        .map(|input| {
            let (input, _) = tag::<&str, &str, Error<&str>>("Sensor at x=")(input).unwrap();
            let (input, x_sensor) = parse_integer(input).unwrap();
            let (input, _) = tag::<&str, &str, Error<&str>>(", y=")(input).unwrap();
            let (input, y_sensor) = parse_integer(input).unwrap();
            let (input, _) =
                tag::<&str, &str, Error<&str>>(": closest beacon is at x=")(input).unwrap();
            let (input, x_beacon) = parse_integer(input).unwrap();
            let (input, _) = tag::<&str, &str, Error<&str>>(", y=")(input).unwrap();
            let (input, y_beacon) = parse_integer(input).unwrap();
            assert!(input.len() == 0, "Size should be 0");
            (
                Beacon {
                    x: x_beacon as i32,
                    y: y_beacon as i32,
                },
                Sensor {
                    x: x_sensor as i32,
                    y: y_sensor as i32,
                },
            )
        })
        .collect::<Vec<(Beacon, Sensor)>>()
}

fn get_distress_freq(x: i64, y: i64) -> i64 {
    x * 4000000 + y
}

fn part2(input: &str, limit: i32) -> i64 {
    let coordinate_pairs = parse(input);
    let mut num_times_seen: BTreeMap<(i32, i32), i32> = BTreeMap::new();
    let min_max_range_shifted_coords = coordinate_pairs
        .iter()
        .map(|(beacon, sensor)| {
            (
                Sensor {
                    x: sensor.x + sensor.y,
                    y: -sensor.x + sensor.y,
                },
                (beacon.x - sensor.x).abs() + (sensor.y - beacon.y).abs(),
            )
        })
        .map(|(sensor, distance)| {
            let max_x_possible = sensor.x + distance + 1;
            let min_x_possible = sensor.x - distance - 1;
            let max_y_possible = sensor.y + distance + 1;
            let min_y_possible = sensor.y - distance - 1;
            (
                (min_x_possible, max_x_possible),
                (min_y_possible, max_y_possible),
            )
        })
        .collect::<Vec<_>>();
    min_max_range_shifted_coords
        .iter()
        .for_each(|((min_x, max_x), (min_y, max_y))| {
            for x in *min_x..=*max_x {
                for y in vec![max(0, *min_y), min(*max_y, limit)] {
                    if let Some(current_val) = num_times_seen.get(&(x, y)) {
                        num_times_seen.insert((x, y), current_val + 1);
                    } else {
                        num_times_seen.insert((x, y), 1);
                    }
                }
            }

            for y in *min_y..=*max_y {
                for x in vec![*min_x, *max_x] {
                    if let Some(current_val) = num_times_seen.get(&(x, y)) {
                        num_times_seen.insert((x, y), current_val + 1);
                    } else {
                        num_times_seen.insert((x, y), 1);
                    }
                }
            }
        });
    let temp_vec = num_times_seen
        .iter()
        .filter(|(_, b)| **b >= 4)
        .filter(|(a, _)| {
            let (x, y) = **a;
            min_max_range_shifted_coords
                .iter()
                .all(|((min_x, max_x), (min_y, max_y))| {
                    let adj_point_interior = [(x - 1, y), (x + 1, y), (x, y - 1), (x, y + 1)]
                        .iter()
                        .all(|(x1, y1)| {
                            min_max_range_shifted_coords.iter().any(
                                |((min_x, max_x), (min_y, max_y))| {
                                    ((*x1 >= *min_x + 1) && (*x1 <= *max_x - 1))
                                        && ((*y1 >= *min_y + 1) && (*y1 <= *max_y - 1))
                                },
                            )
                        });
                    let point_not_in_interior = !(((x >= *min_x + 1) && (x <= *max_x - 1))
                        && ((y >= *min_y + 1) && (y <= *max_y - 1)));
                    point_not_in_interior && adj_point_interior
                })
        })
        .map(|(a, _)| (*a).clone())
        .map(|(x, y)| ((x - y) / 2, (x + y) / 2))
        .filter(|(x, y)| ((*x <= limit && *y <= limit) && (*x >= 0 && *y >= 0)))
        .collect::<Vec<_>>();

    let (x, y) = temp_vec.get(0).unwrap();
    return get_distress_freq(*x as i64, *y as i64);
}

fn main() {
    let input = fs::read_to_string("./input15.txt").expect("Cannot read file to string");
    println!("part1: {:?}", part1(&input, 2000000));
    println!("part2: {:?}", part2(&input, 4000000));
}

#[cfg(test)]
mod tests {
    use crate::*;
    use std::fs;

    #[test]
    fn test_case_dumb() {
        let input = fs::read_to_string("./input15.dev2.txt").expect("Cannot read file to string");
        assert_eq!(part1(&input, 10), 12);
    }

    #[test]
    fn test_case1() {
        let input = fs::read_to_string("./input15.dev.txt").expect("Cannot read file to string");
        assert_eq!(part1(&input, 10), 26);
    }

    #[test]
    fn test_part2() {
        let input = fs::read_to_string("./input15.dev.txt").expect("Cannot read file to string");
        assert_eq!(part2(&input, 20), 56000011);
    }
}
