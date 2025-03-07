"""QMK Doctor

Check out the user's QMK environment and make sure it's ready to compile.
"""
import platform
from subprocess import DEVNULL

from milc import cli
from milc.questions import yesno

from qmk import submodules
from qmk.constants import QMK_FIRMWARE
from .check import CheckStatus, check_binaries, check_binary_versions, check_submodules, check_git_repo


def os_tests():
    """Determine our OS and run platform specific tests
    """
    platform_id = platform.platform().lower()

    if 'darwin' in platform_id or 'macos' in platform_id:
        from .macos import os_test_macos
        return os_test_macos()
    elif 'linux' in platform_id:
        from .linux import os_test_linux
        return os_test_linux()
    elif 'windows' in platform_id:
        from .windows import os_test_windows
        return os_test_windows()
    else:
        cli.log.warning('Unsupported OS detected: %s', platform_id)
        return CheckStatus.WARNING


@cli.argument('-y', '--yes', action='store_true', arg_only=True, help='Answer yes to all questions.')
@cli.argument('-n', '--no', action='store_true', arg_only=True, help='Answer no to all questions.')
@cli.subcommand('Basic QMK environment checks')
def doctor(cli):
    """Basic QMK environment checks.

    This is currently very simple, it just checks that all the expected binaries are on your system.

    TODO(unclaimed):
        * [ ] Compile a trivial program with each compiler
    """
    cli.log.info('QMK Doctor is checking your environment.')
    cli.log.info('CLI version: %s', cli.version)
    cli.log.info('QMK home: {fg_cyan}%s', QMK_FIRMWARE)

    status = os_tests()

    # Make sure our QMK home is a Git repo
    git_ok = check_git_repo()

    if git_ok == CheckStatus.WARNING:
        cli.log.warning("QMK home does not appear to be a Git repository! (no .git folder)")
        status = CheckStatus.WARNING

    # Make sure the basic CLI tools we need are available and can be executed.
    bin_ok = check_binaries()

    if not bin_ok:
        if yesno('Would you like to install dependencies?', default=True):
            cli.run(['util/qmk_install.sh', '-y'], stdin=DEVNULL, capture_output=False)
            bin_ok = check_binaries()

    if bin_ok:
        cli.log.info('All dependencies are installed.')
    else:
        status = CheckStatus.ERROR

    # Make sure the tools are at the correct version
    ver_ok = check_binary_versions()
    if CheckStatus.ERROR in ver_ok:
        status = CheckStatus.ERROR
    elif CheckStatus.WARNING in ver_ok and status == CheckStatus.OK:
        status = CheckStatus.WARNING

    # Check out the QMK submodules
    sub_ok = check_submodules()

    if sub_ok == CheckStatus.OK:
        cli.log.info('Submodules are up to date.')
    else:
        if yesno('Would you like to clone the submodules?', default=True):
            submodules.update()
            sub_ok = check_submodules()

        if sub_ok == CheckStatus.ERROR:
            status = CheckStatus.ERROR
        elif sub_ok == CheckStatus.WARNING and status == CheckStatus.OK:
            status = CheckStatus.WARNING

    # Report a summary of our findings to the user
    if status == CheckStatus.OK:
        cli.log.info('{fg_green}QMK is ready to go')
        return 0
    elif status == CheckStatus.WARNING:
        cli.log.info('{fg_yellow}QMK is ready to go, but minor problems were found')
        return 1
    else:
        cli.log.info('{fg_red}Major problems detected, please fix these problems before proceeding.')
        cli.log.info('{fg_blue}Check out the FAQ (https://docs.qmk.fm/#/faq_build) or join the QMK Discord (https://discord.gg/Uq7gcHh) for help.')
        return 2
