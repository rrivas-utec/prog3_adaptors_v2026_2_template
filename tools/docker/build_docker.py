from docker import from_env, errors
from run_docker import run_container
from close_docker import close_container
# import six


def build_image(image_name):
    # Call to client
    client = from_env()
    if not client.ping():
        print('Client is not ready...')
        return
    # Build the container
    try:
        print(f'Building a local image: [{image_name}]')
        print(f'this will take several minutes...')
        client.images.build(
            path='.',
            nocache=True,
            tag=image_name,
            rm=True,
            forcerm=True
        )
        print(f'Image: [{image_name}] has been created successfully...')
    except errors.BuildError:
        print(f'ERROR: Cannot build image: [{image_name}]...')


def remove_image(image_name):
    # Call to client
    client = from_env()
    try:
        client.images.get(image_name)
        client.images.remove(image=image_name, force=True)
    except errors.ImageNotFound:
        pass


if __name__ == '__main__':
    rep_name = 'rudrivas/cpp_docker_environment'
    img_name = 'cpp_docker_environment'
    cnt_name = 'cpp_docker_environment'

    close_container(cnt_name)
    remove_image(img_name)
    build_image(img_name)
    run_container(img_name, cnt_name)
    remove_image(rep_name)
