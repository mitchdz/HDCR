# Building

Instructions for building and configuring hdcr

## Step 1 - Dependencies


## Step 2 - Bootstrapping

Run the `bootstrap` command.

```sh
$ ./bootstrap
```

## Step 3 - Configuring

For people wanting to just build hdcr, run:
```sh
./configure
```

## Step 4 - Building

The next step after [configuring](#step-3---configuring) is to compile the source code.

This can be done by simply running:
```sh
make all
```

### Configure Options
1. `--enable-unit` - Enables the unit tests when running `make check`


## Step 5 - Testing

To enable testing, run `make check`.

**Note:** If make check runs 0 tests, you likely need the configure options `--enable-unit` and `--enable-integration`. See [Configure Options](#configure-options)
