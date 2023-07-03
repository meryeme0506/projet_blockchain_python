import bip
import sys

def create_mnemonic_phrase(arg):
    bip.create_recovery_phrase(arg)

def validate_recovery_phrase(arg):
    bip.validateRecoveryPhrase(arg)

def usage():
    print('Usage : {:s} x'.format(sys.argv[0]))
    print('with x either a number in the list [12, 15, 18, 21, 24] or a mnemonic phrase (with quotes)')
    exit(1)


if __name__ == "__main__":

    if len(sys.argv) != 2:
        usage()
    else:
        if int(x) in [12, 15, 18, 21, 24]:
            try:
                x = int(sys.argv[1])
                create_mnemonic_phrase(x)
            except ValueError:
                usage()
        else:
            try:
                x = str(sys.argv[1])
                validate_recovery_phrase(x)
            except Exception:
                usage()
