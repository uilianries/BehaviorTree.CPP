#!/usr/bin/env python
# -*- coding: utf-8 -*-

import os
import re
from cpt.packager import ConanMultiPackager
from cpt.ci_manager import CIManager
from cpt.printer import Printer


class BuilderSettings(object):
    @property
    def username(self):
        """ Set catchorg as package's owner
        """
        return os.getenv("CONAN_USERNAME", "BehaviorTree")

    @property
    def login_username(self):
        """ Set Bintray login username
        """
        return os.getenv("CONAN_LOGIN_USERNAME", "facontidavide")

    @property
    def upload(self):
        """ Set Catch2 repository to be used on upload.
            The upload server address could be customized by env var
            CONAN_UPLOAD. If not defined, the method will check the branch name.
            Only master or CONAN_STABLE_BRANCH_PATTERN will be accepted.
            The master branch will be pushed to testing channel, because it does
            not match the stable pattern. Otherwise it will upload to stable
            channel.
        """
        return os.getenv("CONAN_UPLOAD", "https://api.bintray.com/conan/facontidavide/conan")

    @property
    def remotes(self):
        """ cppzmq is not approved on Conan center yet
        """
        return os.getenv("CONAN_REMOTES", "https://api.bintray.com/conan/bincrafters/public-conan")

    @property
    def upload_only_when_stable(self):
        """ Force to upload when running over tag branch
        """
        return os.getenv("CONAN_UPLOAD_ONLY_WHEN_STABLE", "True").lower() in ["true", "1", "yes"]

    @property
    def stable_branch_pattern(self):
        """ Only upload the package the branch name is like a tag
        """
        return os.getenv("CONAN_STABLE_BRANCH_PATTERN", r"v\d+\.\d+\.\d+")

    @property
    def reference(self):
        """ Read project version from branch create Conan referece
        """
        return os.getenv("CONAN_REFERENCE", "BehaviorTree.CPP/{}@{}/{}".format(self._branch, self.username, self.channel))

    @property
    def channel(self):
        """ Default Conan package channel when not stable
        """
        return os.getenv("CONAN_CHANNEL", "testing")

    @property
    def _branch(self):
        """ Get branch name from CI manager
        """
        printer = Printer(None)
        ci_manager = CIManager(printer)
        return ci_manager.get_branch()


if __name__ == "__main__":
    settings = BuilderSettings()
    builder = ConanMultiPackager(
        reference=settings.reference,
        channel=settings.channel,
        upload=settings.upload,
        remotes=settings.remotes,
        upload_only_when_stable=settings.upload_only_when_stable,
        stable_branch_pattern=settings.stable_branch_pattern,
        login_username=settings.login_username,
        username=settings.username,
        test_folder=os.path.join(".conan", "test_package"))
    builder.add()
    builder.run()
