#!/usr/bin/env python3
import argparse
import subprocess
import sys
import tempfile
import time
from dataclasses import dataclass
from pathlib import Path
from typing import Optional
from PIL import Image

ROOT = Path(__file__).resolve().parent.parent
BUILD_DIR = ROOT / "build"
APP_PATH = BUILD_DIR / "Resizer"
INPUT_IMAGE = Path(__file__).resolve().parent / "test-cat.jpg"
OUTPUT_DIR = Path(__file__).resolve().parent / "output"


def run_app(app_path, args, stdin, input_delay=0.0):
    if not app_path.exists():
        raise FileNotFoundError(f"Resizer executable not found at {app_path}")

    if input_delay and stdin:
        process = subprocess.Popen(
            [str(app_path), *args],
            stdin=subprocess.PIPE,
            stdout=subprocess.PIPE,
            stderr=subprocess.PIPE,
            text=True,
        )
        for chunk in stdin.splitlines(keepends=True):
            process.stdin.write(chunk)
            process.stdin.flush()
            time.sleep(input_delay)
        stdout, stderr = process.communicate(timeout=20)
        return subprocess.CompletedProcess(
            args=[str(app_path), *args],
            returncode=process.returncode,
            stdout=stdout,
            stderr=stderr,
        )

    process = subprocess.run(
        [str(app_path), *args],
        input=stdin,
        stdout=subprocess.PIPE,
        stderr=subprocess.PIPE,
        text=True,
        timeout=20,
    )
    return process


def assert_contains(text, fragment, message):
    if fragment not in text:
        raise AssertionError(f"{message}: expected {fragment!r} in output\nActual output:\n{text}")


def test_invalid_arguments(app_path, input_image):
    process = run_app(app_path, [], "")
    assert process.returncode != 0, "Expected nonzero exit code for missing arguments"
    assert_contains(process.stdout + process.stderr, "Usage", "Missing usage output")
    print("PASS: invalid arguments")


def test_input_file_not_found(app_path):
    process = run_app(app_path, ["-i", "does_not_exist.jpg"], "")
    assert process.returncode != 0, "Expected nonzero exit code for missing input file"
    assert_contains(process.stderr, "ERROR: Input file", "Expected missing-file error")
    assert_contains(process.stdout, "Usage", "Expected usage output on missing input file")
    print("PASS: input file not found")


def test_quit_without_changes(app_path, input_image):
    process = run_app(app_path, ["-i", str(input_image)], "6\n")
    assert process.returncode == 0, "Expected clean exit when quitting immediately"
    assert_contains(process.stdout, "WHAT DO YOU WANT TO DO", "Expected menu to be printed")
    print("PASS: quit without changes")


def test_save_no_changes(app_path, input_image):
    process = run_app(app_path, ["-i", str(input_image)], "5\n6\n")
    assert process.returncode == 0, "Expected clean exit after save without changes"
    assert_contains(process.stdout, "Nothing changed in the image - save aborted", "Expected no-save message")
    print("PASS: save without changes")

@dataclass
class SaveConfig:
    app_path: Path
    input_image: Path
    output_prefix: Path
    inputs: str
    input_delay: float = 0.0


def run_save(config: SaveConfig):
    expected_saved_file = config.output_prefix.with_suffix(".jpg")
    process = run_app(
        config.app_path,
        ["-i", str(config.input_image), "-o", str(config.output_prefix)],
        config.inputs,
        input_delay=config.input_delay,
    )

    assert process.returncode == 0, "Expected clean exit after resizing and saving"
    assert expected_saved_file.exists(), f"Expected saved file at {expected_saved_file}"
    assert expected_saved_file.stat().st_size > 0, "Saved output file should not be empty"


def assert_saved_image_size(image_path: Path, expected_size: tuple[int, int]):
    with Image.open(image_path) as saved_image:
        assert saved_image.size == expected_size, (
            f"Expected saved image size {expected_size}, got {saved_image.size}"
        )


def test_save_resize(app_path, input_image, save_output=False):
    expected_size = (50, 50)
    inputs = "1\n50\n50\n5\n6\n"

    if save_output:
        OUTPUT_DIR.mkdir(parents=True, exist_ok=True)
        output_prefix = OUTPUT_DIR / "resized_image"
        run_save(SaveConfig(app_path, input_image, output_prefix, inputs))
        assert_saved_image_size(output_prefix.with_suffix('.jpg'), expected_size)
        print(f"PASS: save after resize -> {output_prefix.with_suffix('.jpg')}")
    else:
        with tempfile.TemporaryDirectory() as tmp_dir:
            output_prefix = Path(tmp_dir) / "resized_image"
            run_save(SaveConfig(app_path, input_image, output_prefix, inputs))
            assert_saved_image_size(output_prefix.with_suffix('.jpg'), expected_size)
            print("PASS: save after resize")


def test_save_crop(app_path, input_image, save_output=False):
    expected_size = (200, 200)
    inputs = "2\n400\n400\n600\n600\n5\n6\n"

    if save_output:
        OUTPUT_DIR.mkdir(parents=True, exist_ok=True)
        output_prefix = OUTPUT_DIR / "cropped_image"
        run_save(SaveConfig(app_path, input_image, output_prefix, inputs))
        assert_saved_image_size(output_prefix.with_suffix('.jpg'), expected_size)
        print(f"PASS: save after crop -> {output_prefix.with_suffix('.jpg')}")
    else:
        with tempfile.TemporaryDirectory() as tmp_dir:
            output_prefix = Path(tmp_dir) / "cropped_image"
            run_save(SaveConfig(app_path, input_image, output_prefix, inputs))
            assert_saved_image_size(output_prefix.with_suffix('.jpg'), expected_size)
            print("PASS: save after crop")


def test_save_shrink_file_size(app_path, input_image, save_output=False):
    target_bytes = 10000
    inputs = f"3\n{target_bytes}\n5\n6\n"

    if save_output:
        OUTPUT_DIR.mkdir(parents=True, exist_ok=True)
        output_prefix = OUTPUT_DIR / "shrinked_image"
        run_save(SaveConfig(app_path, input_image, output_prefix, inputs, input_delay=0.05))
        saved_file = output_prefix.with_suffix('.jpg')
        assert saved_file.stat().st_size <= target_bytes, (
            f"Expected shrunk file size <= {target_bytes}, got {saved_file.stat().st_size}"
        )
        print(f"PASS: save after shrink -> {saved_file}")
    else:
        with tempfile.TemporaryDirectory() as tmp_dir:
            output_prefix = Path(tmp_dir) / "shrinked_image"
            run_save(SaveConfig(app_path, input_image, output_prefix, inputs, input_delay=0.05))
            saved_file = output_prefix.with_suffix('.jpg')
            assert saved_file.stat().st_size <= target_bytes, (
                f"Expected shrunk file size <= {target_bytes}, got {saved_file.stat().st_size}"
            )
            print("PASS: save after shrink")


def main():
    parser = argparse.ArgumentParser(description="Run app-level Resizer tests.")
    parser.add_argument("--app", type=Path, default=APP_PATH, help="Path to the Resizer executable")
    parser.add_argument("--input", type=Path, default=INPUT_IMAGE, help="Path to the input image")
    parser.add_argument(
        "--save-output",
        action="store_true",
        help="Save modified files to testing/output instead of a temporary directory",
    )
    args = parser.parse_args()

    app_path = args.app
    input_image = args.input
    save_output = args.save_output

    tests = [
        lambda: test_invalid_arguments(app_path, input_image),
        lambda: test_input_file_not_found(app_path),
        lambda: test_quit_without_changes(app_path, input_image),
        lambda: test_save_no_changes(app_path, input_image),
        lambda: test_save_resize(app_path, input_image, save_output=save_output),
        lambda: test_save_crop(app_path, input_image, save_output=save_output),
        lambda: test_save_shrink_file_size(app_path, input_image, save_output=save_output),
    ]

    for test in tests:
        test()

    print("All app-level tests passed.")


if __name__ == "__main__":
    main()
