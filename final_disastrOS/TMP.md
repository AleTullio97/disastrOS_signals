###### General guidelines:
- All projects should be hosted on github/gitlab or similars.
- Each student performs commits using his own account.
- We want to see the individual commits. Projects with a single commit are considered invalid.
- The contribution of each member in the group to the development
  should be crystal clear. Comment your sections in the source with some
  initials or nickname

### Projects
7. **Signals in disastrOS** ---> `[x1]` `[C98]` `[thesis]`

  You should implement an IPC mechanism.

  Each process has a `signal` variable in the PCB and each specific signal
  should be represented as a bitmask to be set in the `signal` variable.
		
  Each signal might have an handler. 
  Signals of the same process share a common stack.

  Each installed signal should be managed by an appropriate context that,
  when terminated, returns to the context of the process.

  A signal is checked when a process enters the running state, and if so the 
  context switch should occur not on the process context but on the signal context.
  
  While serving a signal, other signals of the same type are ignored.

  The starting point for this project is the version of disastrOS presented in class -
  available [here](https://gitlab.com/grisetti/sistemi_operativi_2019_20/-/tree/master/source/08_disastrOS/disastrOS_04_resources).



### Final Comments

When you finish the project, you should send an email to both the professor
**and** the tutor. Note that you should provide also a *very brief* report
on the project (max 2 pages), reporting
- what
- how
- how to run.

You should include this document in your repository.
