# <u>Contributing to libiolink</u>

First off, thanks for taking the time to contribute! Every contribution to the project is more than welcome, no matter how small it is. The following is a set of guidelines for contributing to libiolink, which is hosted on GitHub. These are mostly guidelines, not rules. Use your best judgment, and feel free to propose changes to this document in a pull request.

# <u>Code of Conduct</u>

This Code of Conduct is adapted from the Contributor Covenant, version 2.0,

available at [https://www.contributor-covenant.org/version/2/0/code_of_conduct.html](https://www.contributor-covenant.org/version/2/0/code_of_conduct.html)

By participating, you are expected to uphold this code. Please report unacceptable behavior to [libiolink@github.com](mailto:libiolink@github.com).

# <u>How Can I Contribute?</u>

There are many ways to contribute to the project and make it even better. But I particularly need help in these areas:

1. Send me the `devicetree` for some unimplemented master, so I can
   
   create a driver for it

2. Improve the documentation or give me some hint what can be improved

3. Improve the examples or give me some hint what can be improved

4. You can file a bug report

5. You can file a feature request

6. Fix a bug and send me the patch

7. Implement a new feature

8. Write a new driver for a master

9. Write a new driver for a IO-Link device

# <u>Participate in development</u>

## Git Workflow

The git branching strategy is based on the OneFlow model proposed by Adam Ruka. The original article can be found [here](https://www.endoflineblog.com/oneflow-a-git-branching-model-and-workflow).

In an essence there is one mainline branch called `develop`  and all the others branch out of it and then they merge into it. There is also a `master` branch that follows the latest release. All the others are support branches that are not supposed to be pushed to the origin except in rare cases when it is absolutely necessary to do so.

### Master branch `master`

It's main purpuse is to point to the latest release tag. This is the de facto standard.

### Develop branch `develop`

This is the main ethernal branch. The whole development process happens here. Every commit must be in a working state.

### Feature branch `feature/my-feature`

Branches from develop. Merges into develop.

> Feature branches (also sometimes called topic branches) are where the
>  day-to-day development work happens - hence, they are by far the most 
> common of all the support branches. They are used to develop new 
> features and bugfixes for the upcoming release. They are named 
> to `feature/my-feature`.
> 
> Feature branches often exist only in the developer's repository, and 
> are never pushed - however, if there are multiple people working on one 
> feature, or if the feature will take a long time to develop, it's 
> typical to push them to the central repository (if only to make sure the
>  code isn't lost with a single disk failure).

### Hotfix branch `hotfix/my-feature`

Branches from the commit tagged with the buggy version. Merges into develop. When the merging is ready, you have to Fast Forward the `master` branch to the latest release `tag`. Hotfixes bump the Patch number.

> Hotfix branches are very similar to release branches - they result in
>  a new version of the project being released. Where they differ is their
>  intentions - while release branches signify a planned production 
> milestone, hotfix branches are most often an unwanted but necessary 
> exception to the usual release cadence, typically because of some 
> critical defect found in the latest release that needs to be fixed as 
> soon as possible.

### Release branch `release/version`

Branches from develop. Merges into develop. When merging is ready, you have to Fast Forward the `master` branch to the latest release `tag`.

> Release branches are created to prepare the software for being released. This could be as simple as bumping the version number in the 
> configuration, or involve things like code freezes, producing Release 
> Candidates, and having a full QA process. The important thing is all 
> that happens on a separate branch, so that day-to-day development can 
> continue as usual on `develop`.

## Development Environment

Depending on what you want to achieve, there are a couple of different scenarios for setting up a Development Environment(DE):

- The simplest approach is to use a simple text editor. This is DE is only usefull if there is some obvious bug in the code, that can be squashed easily without further testing. An example of such bug is a typo in the comments or something trivial in the code.

- The more advanced approach is to create a simple test project and to include the library or to open some of the examples. This DE is much better than the previous because that way you can test: how the library compiles when integrated into a larger project; is the compiler throwing any errors or warnings; triggering of static asserts, etc.

- The recommended approach is the second one plus a hardware environment where you can test your code in a real world scenario. An example DE includes:
  good IDE, some example project that compiles successfully, IFM master, **IO-Link** device connected to the master, a network connection between the master and the development host.

## Code Organization

# <u>Versioning</u>

The software uses "Semantic Versioning" as described here: [https://semver.org/](https://semver.org/)

Given a version number MAJOR.MINOR.PATCH, incrementing the:

- MAJOR version bump will introduce incompatible API changes

- MINOR version bump will add functionality in a backwards compatible manner

- PATCH version bump will make backwards compatible bug fixes.

Additional labels for pre-release and build metadata are available as extensions to the MAJOR.MINOR.PATCH format.
