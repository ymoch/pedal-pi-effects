# pedal-pi-effects

Learning for [Pedal-Pi by ElectroSmash](https://www.electrosmash.com/pedal-pi)

## Provision

Using [Python venv](https://docs.python.org/3/library/venv.html) is strongly recommended.

```sh
pip install "fabric2>=2.4,<3"
pip install "patchwork>=1.0,<2"
pip install cryptography==2.4.2  # To reduce warnings.

fab2 -H user@host provision
```
