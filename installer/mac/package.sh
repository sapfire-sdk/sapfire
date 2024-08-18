#!/bin/bash

pkgbuild --root "$1" --identifier com.sapfire-sdk.sapfire --scripts "installer/mac/Scripts" --install-location "/tmp/sapfire-install" $2
