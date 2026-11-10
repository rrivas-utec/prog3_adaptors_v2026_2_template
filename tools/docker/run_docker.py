from docker import from_env
import six


# Run
def run_container(image_name, container_name):
    # container_name = 'cpp_docker_environment'
    client = from_env()
    lst = client.containers.list(f'name:{container_name}')
    if not lst:
        client.containers.run(
            image=image_name,
            name=container_name,
            detach=True,
            ports={'22/tcp': ('127.0.0.1', 2222)},
            cap_add=['sys_ptrace']
        )
        print(f'Container: [{container_name}] has been started successfully...')
    else:
        print(f'Container: [{container_name}] is running...')


if __name__ == '__main__':
    run_container('cpp_docker_environment', 'cpp_docker_environment')
