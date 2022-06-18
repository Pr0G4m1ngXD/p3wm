# Contributing to p3wm
First of all, Thank you for investing your time contributing to p3wm and making it better!

Read out [Code of Conduct](./CODE_OF_CONDUCT.md) to keep this community approachable and respectable.

Here we will guide you from an overview of the contribution workflow

## New contributor guide
To get an overview of the project, read the [README](./README.md). Here are some resources to help you get started with open source contributions:

- [Finding ways to contribute to open source on GitHub](https://docs.github.com/en/get-started/exploring-projects-on-github/finding-ways-to-contribute-to-open-source-on-github)
- [Set up Git](https://docs.github.com/en/get-started/quickstart/set-up-git)
- [GitHub flow](https://docs.github.com/en/get-started/quickstart/github-flow)
- [Collaborating with pull requests](https://docs.github.com/en/github/collaborating-with-pull-requests)

## Contribution Workflow

### Issues

#### Create a new issue
If you find an issue with this repository or want to request a feature, [search if an issue already exists](https://docs.github.com/en/github/searching-for-information-on-github/searching-on-github/searching-issues-and-pull-requests#search-by-the-title-body-or-comments). If not you can open one.

#### Resolve an issue
Look through our [existing issues](https://github.com/github/docs/issues) and find one that intrests you. You can narrow down the search using labels as filters. If you find an issue you want to work on, you're welcome to open a PR with a fix.

### Making changes

You will need to have the xlib dependency and xterm installed

Clone the reposity locally
*     $ git clone https://github.com/Pr0G4m1ngXD/p3wm
*     $ cd p3wm

Copy the config file in defaults/config.conf to /usr/share/p3wm/config.conf
*     $ sudo mkdir /usr/share/p3wm
*     $ sudo cp defaults/config.conf /usr/share/p3wm/config.conf

Build the project in the src directory
*     $ cd src
*     $ make

Run the build (Make sure that you can run xinit without sudo/root)
*     $ cd ..
*     $ xinit ./start.sh
(I have used x11vnc due to me not being able to dualboot a distibution, as i require windows for some software/games)

Start p3wm from the open Terminal (i have not put this in start.sh so you can see the logs)
*     $ ./p3wm.x11

### Pull Request

When you're finished with the changes, create a pull request, also known as a PR.
- Fill the "Ready for review" template so that we can review your PR. This template helps reviewers understand your changes as well as the purpose of your pull request. 
- Don't forget to [link PR to issue](https://docs.github.com/en/issues/tracking-your-work-with-issues/linking-a-pull-request-to-an-issue) if you are solving one.
- Enable the checkbox to [allow maintainer edits](https://docs.github.com/en/github/collaborating-with-issues-and-pull-requests/allowing-changes-to-a-pull-request-branch-created-from-a-fork) so the branch can be updated for a merge.
Once you submit your PR, a Docs team member will review your proposal. We may ask questions or request for additional information.
- We may ask for changes to be made before a PR can be merged, either using [suggested changes](https://docs.github.com/en/github/collaborating-with-issues-and-pull-requests/incorporating-feedback-in-your-pull-request) or pull request comments. You can apply suggested changes directly through the UI. You can make any other changes in your fork, then commit them to your branch.
- As you update your PR and apply changes, mark each conversation as [resolved](https://docs.github.com/en/github/collaborating-with-issues-and-pull-requests/commenting-on-a-pull-request#resolving-conversations).
- If you run into any merge issues, checkout this [git tutorial](https://github.com/skills/resolve-merge-conflicts) to help you resolve merge conflicts and other issues.

### Your PR is merged!

🎉Congratulations🎉 The p3wm team thanks you. 

Once your PR is merged, your contributions will be available in the next release of p3wm.

## Credits
This CONTRIBUTING.md file was modelled after the [github/docs/CONTRIBUTING.md](https://github.com/github/docs/blob/main/CONTRIBUTING.md) file, and we thank the original author.
