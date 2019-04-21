import os

from invoke import task
from patchwork.transfers import rsync


@task
def install_bcm2835(c):
    """
    See: https://www.electrosmash.com/forum/pedal-pi/202-how-to-start-programming-pedal-pi
    """
    url = 'http://www.airspayce.com/mikem/bcm2835/bcm2835-1.58.tar.gz'
    dir_name = 'bcm2835-1.58'

    c.run(f'rm -rf {dir_name}')

    c.run(f'curl -s "{url}" | tar zxf -')
    with c.cd(dir_name):
        c.run('./configure')
        c.run('make')

    c.sudo(f'bash -c \'cd "{dir_name}" && make install\'')

    c.run(f'rm -rf {dir_name}')


@task
def install_cmake(c):
    c.sudo('apt-get install -y cmake')


@task
def install_pedal_pi_effects(c):
    local_dir_path = os.path.dirname(__file__)
    dir_name = 'pedal-pi-effects'

    c.run(f'rm -rf {dir_name}')
    rsync(c, f'{local_dir_path}/', f'{dir_name}/',
        exclude=['.git'], rsync_opts='-C --filter=":- .gitignore"')
    with c.cd(dir_name):
        c.run('cmake .')
        c.run('make')

    c.sudo(f'bash -c \'cd "{dir_name}" && make install\'')


@task
def provision(c):
    install_bcm2835(c)
    install_cmake(c)
