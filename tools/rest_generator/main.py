import argparse

if __name__ == '__main__':
    parser = argparse.ArgumentParser(prog='RestGenerator')
    parser.add_argument('--sources', type=str, nargs='+',
                        help='')
    parser.add_argument('--output_dir', nargs=1,
                        help='')

    args = parser.parse_args()
