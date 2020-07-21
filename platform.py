# Copyright 2014-present PlatformIO <contact@platformio.org>
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#    http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

import json
import os

from platformio.managers.platform import PlatformBase
from platformio.util import get_systype


class FreescalekinetisPlatform(PlatformBase):

    def is_embedded(self):
        return True

    def configure_default_packages(self, variables, targets):
        board = variables.get("board")
        frameworks = variables.get("pioframework", [])
        if "mbed" in frameworks:
            deprecated_boards_file = os.path.join(
                self.get_dir(), "misc", "mbed_deprecated_boards.json")
            if os.path.isfile(deprecated_boards_file):
                with open(deprecated_boards_file) as fp:
                    if board in json.load(fp):
                        self.packages["framework-mbed"]["version"] = "~6.51504.0"
            self.packages["toolchain-gccarmnoneeabi"]["version"] = "~1.90201.0"

        if "zephyr" in frameworks:
            for p in self.packages:
                if p.startswith("framework-zephyr-") or p in (
                    "tool-cmake", "tool-dtc", "tool-ninja"):
                    self.packages[p]["optional"] = False
            if "windows" not in get_systype():
                self.packages['tool-gperf']['optional'] = False

        jlink_conds = [
            "jlink" in variables.get(option, "")
            for option in ("upload_protocol", "debug_tool")
        ]
        if board:
            board_config = self.board_config(board)
            jlink_conds.extend([
                "jlink" in board_config.get(key, "")
                for key in ("debug.default_tools", "upload.protocol")
            ])
        jlink_pkgname = "tool-jlink"
        if not any(jlink_conds) and jlink_pkgname in self.packages:
            del self.packages[jlink_pkgname]

        return PlatformBase.configure_default_packages(self, variables,
                                                       targets)

    def get_boards(self, id_=None):
        result = PlatformBase.get_boards(self, id_)
        if not result:
            return result
        if id_:
            return self._add_default_debug_tools(result)
        else:
            for key, value in result.items():
                result[key] = self._add_default_debug_tools(result[key])
        return result

    def _add_default_debug_tools(self, board):
        debug = board.manifest.get("debug", {})
        upload_protocols = board.manifest.get("upload", {}).get(
            "protocols", [])
        if "tools" not in debug:
            debug['tools'] = {}

        # CMSIS-DAP / BlackMagic Probe
        for link in ("blackmagic", "cmsis-dap", "jlink"):
            if link not in upload_protocols or link in debug['tools']:
                continue

            if link == "blackmagic":
                debug['tools']['blackmagic'] = {
                    "hwids": [["0x1d50", "0x6018"]],
                    "require_debug_port": True
                }

            elif link == "cmsis-dap":
                pyocd_target = debug.get("pyocd_target")
                assert pyocd_target

                debug['tools'][link] = {
                    "onboard": True,
                    "server": {
                        "package": "tool-pyocd",
                        "executable": "$PYTHONEXE",
                        "arguments": [
                            "pyocd-gdbserver.py",
                            "-t",
                            pyocd_target
                        ]
                    }
                }

            elif link == "jlink":
                assert debug.get("jlink_device"), (
                    "Missed J-Link Device ID for %s" % board.id)
                debug['tools'][link] = {
                    "server": {
                        "package": "tool-jlink",
                        "arguments": [
                            "-singlerun",
                            "-if", "SWD",
                            "-select", "USB",
                            "-device", debug.get("jlink_device"),
                            "-port", "2331"
                        ],
                        "executable": ("JLinkGDBServerCL.exe"
                                       if "windows" in get_systype() else
                                       "JLinkGDBServer")
                    }
                }

        board.manifest['debug'] = debug
        return board
