TEMPLATE = subdirs
 
SUBDIRS = \
          tucuxi-core \
          tucuxi-tuberxpert

# where to find the sub projects - give the folders
tucuxi-core.subdir  = tucuxi-core/make/qtcreator/tucucli
tucuxi-tuberxpert.subdir  = tucuxi-tuberxpert
 
# what subproject depends on others
tucuxi-tuberxpert.depends = tucuxi-core

  