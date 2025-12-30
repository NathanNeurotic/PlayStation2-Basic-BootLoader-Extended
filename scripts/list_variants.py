#!/usr/bin/env python3
import itertools
import json


def main():
    modes = [
        ("normal", {"chainload": 0, "runtime": 0}),
        ("chainload", {"chainload": 1, "runtime": 0}),
        ("runtime", {"chainload": 0, "runtime": 1}),
        ("runtime_chainload", {"chainload": 1, "runtime": 1}),
    ]

    devices = [
        {"name": "HDD", "enable": "HDD", "chainload": "HDD_CHAINLOAD", "runtime": "HDD_RUNTIME"},
        {"name": "MMCE", "enable": "MMCE", "chainload": "MMCE_CHAINLOAD", "runtime": "MMCE_RUNTIME"},
        {"name": "MX4SIO", "enable": "MX4SIO", "chainload": "MX4SIO_CHAINLOAD", "runtime": "MX4SIO_RUNTIME"},
        {"name": "USB", "enable": "USB", "chainload": "USB_CHAINLOAD", "runtime": "USB_RUNTIME"},
        {"name": "XFROM", "enable": "XFROM", "chainload": "XFROM_CHAINLOAD", "runtime": "XFROM_RUNTIME"},
    ]

    flag_order = ["CHAINLOAD", "RUNTIME"]
    for dev in devices:
        flag_order.append(dev["enable"])
        flag_order.append(dev["chainload"])
        if dev.get("runtime"):
            flag_order.append(dev["runtime"])

    def render_flags(flags):
        return " ".join(f"{key}={flags[key]}" for key in flag_order if key in flags)

    include = []

    for mode_name, mode in modes:
        flags = {"CHAINLOAD": mode["chainload"], "RUNTIME": mode["runtime"]}
        include.append({"name": f"regular-{mode_name}", "flags": render_flags(flags)})

    for subset_size in range(1, len(devices) + 1):
        for subset in itertools.combinations(devices, subset_size):
            subset_names = {d["name"] for d in subset}
            if "MMCE" in subset_names and "MX4SIO" in subset_names:
                continue
            for mode_combo in itertools.product(modes, repeat=subset_size):
                flags = {}
                tokens = []
                chainload_requested = False
                for dev, (mode_name, mode_config) in zip(subset, mode_combo):
                    tokens.append(f"{dev['name'].lower()}-{mode_name}")
                    flags[dev["enable"]] = 1
                    if mode_config["chainload"]:
                        flags[dev["chainload"]] = 1
                        chainload_requested = True
                    if mode_config["runtime"]:
                        runtime_flag = dev.get("runtime")
                        if runtime_flag:
                            flags[runtime_flag] = 1
                        else:
                            flags["RUNTIME"] = 1
                if chainload_requested:
                    flags["CHAINLOAD"] = 1
                include.append({"name": "+".join(tokens), "flags": render_flags(flags)})

    print(json.dumps({"include": include}, sort_keys=False))


if __name__ == "__main__":
    main()
