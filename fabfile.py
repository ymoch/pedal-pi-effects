import os

from invoke import task
from patchwork.transfers import rsync


PROJECT_DIR_NAME = 'pedal-pi-effects'
SERVICE_NAME = 'pedal-pi-effects.service'


@task
def install_cmake(c):
    c.sudo('apt-get install -y cmake')


@task
def install_bcm2835(c):
    # See: https://www.electrosmash.com/forum/pedal-pi/202-how-to-start-programming-pedal-pi
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
def install_googletest(c):
    url = 'https://codeload.github.com/google/googletest/tar.gz/release-1.8.1'
    dir_name = 'googletest-release-1.8.1'

    c.run(f'rm -rf {dir_name}')
    c.run(f'curl -s "{url}" | tar zxf -')
    with c.cd(dir_name):
        c.run('cmake .')
        c.run('make')
    c.sudo(f'bash -c \'cd "{dir_name}" && make install\'')

    c.run(f'rm -rf {dir_name}')


@task
def upload(c, target=PROJECT_DIR_NAME, clean_build=False):
    local_dir_path = os.path.dirname(__file__)

    if clean_build:
        c.run(f'rm -rf {target}')
    rsync(c, f'{local_dir_path}/', f'{target}/',
        exclude=['.git'], rsync_opts='-C --filter=":- .gitignore"')


@task
def install(c, clean_build=False):
    dir_name = PROJECT_DIR_NAME

    upload(c, target=dir_name, clean_build=clean_build)
    with c.cd(dir_name):
        c.run('cmake .')
        c.run('make pedal-pi-effects')

    c.sudo(f'bash -c \'cd "{dir_name}" && make install\'')


@task
def ensure_service(c, clean_build=False):
    dir_name = PROJECT_DIR_NAME
    service_name = SERVICE_NAME

    if c.run(f'systemctl list-unit-files | grep "{service_name}"', warn=True).ok:
        return

    upload(c, target=dir_name, clean_build=clean_build)
    c.sudo(f'cp "{dir_name}/etc/service/{service_name}" /etc/systemd/system/')
    c.sudo(f'systemctl enable {service_name}')


@task
def restart(c):
    service_name = SERVICE_NAME

    c.sudo(f'systemctl restart {service_name}')


@task
def deploy(c, clean_build=False):
    service_name = SERVICE_NAME

    upload(c, clean_build=clean_build)
    install(c, clean_build=False)

    ensure_service(c, clean_build=False)
    restart(c)


@task
def provision(c):
    install_cmake(c)
    install_bcm2835(c)
    install_googletest(c)
