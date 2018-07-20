from scorep_tool.model import RegionLoad


class FilterFile:
    """Loads filter from file and saves new filter file.
    """

    def __init__(self, name):
        self.name = name
        self.content = ''
        self.file = None

        self.file_begin = 'SCOREP_REGION_NAMES_BEGIN'
        self.file_end = 'SCOREP_REGION_NAMES_END'

        self.include = 'INCLUDE'
        self.exclude = 'EXCLUDE'

        self.load_regions = []

        self.is_excl = False
        self.is_mangled = True

        self.exclude_marker = False
        self.include_marker = False

        self.regions = []

        self.save_separate = False
        self.save_exclude_only = True

    def load(self):
        """open filter file and save regions rules"""
        with open(self.name) as self.file:
            content = []
            sep = '#'

            for line in self.file:
                if line[0] == sep:
                    continue
                line = line.strip(' \t\n\r')
                line = line.split(sep, 1)[0]
                tmp = line.split(' ')
                for t in tmp:
                    if t:
                        content.append(t)

            for param in content:
                if param == self.file_end:
                    break
                elif param == self.file_begin:
                    continue
                elif param == 'INCLUDE':
                    self.is_excl = False
                    self.is_mangled = False
                elif param == 'EXCLUDE':
                    self.is_excl = True
                    self.is_mangled = False
                elif param == 'MANGLED':
                    self.is_mangled = True
                else:
                    region = RegionLoad(self.is_excl)
                    if self.is_mangled:
                        region.mangled_name = param
                    else:
                        region.demangled_name = param
                    self.load_regions.append(region)

        self.file.closed

    def save(self):
        """create filter file with regions rules"""
        self.file = open(self.name, 'w')
        self.file.write(self.file_begin)
        self.file.write('\n')

        if self.save_separate:
            exclude_list = [i for i in self.regions if i.exclude]
            include_list = [i for i in self.regions if not i.exclude]

            for region in exclude_list:
                if self.exclude_marker:
                    self.file.write('{}'.format(region.mangled_name).rjust(8 + len(region.mangled_name)))
                else:
                    self.file.write('EXCLUDE {}'.format(region.mangled_name))
                    self.exclude_marker = True
                self.file.write('\n')

            if not self.save_exclude_only:
                for region in include_list:
                    if self.include_marker:
                        self.file.write('{}'.format(region.mangled_name).rjust(8 + len(region.mangled_name)))
                    else:
                        self.file.write('INCLUDE {}'.format(region.mangled_name))
                        self.include_marker = True
                    self.file.write('\n')
        else:
            for region in self.regions:
                if region.filtered or region.exclude:
                    if self.exclude_marker:
                        self.file.write('{}'.format(region.mangled_name).rjust(
                            8 + len(region.mangled_name)))
                    else:
                        self.file.write('EXCLUDE {}'.format(region.mangled_name))
                        self.exclude_marker = True
                        self.include_marker = False
                    self.file.write('\n')
                elif not self.save_exclude_only:
                    if self.include_marker:
                        self.file.write('{}'.format(region.mangled_name).rjust(
                            8 + len(region.mangled_name)))
                    else:
                        self.file.write('INCLUDE {}'.format(region.mangled_name))
                        self.include_marker = True
                        self.exclude_marker = False
                    self.file.write('\n')

        self.file.write(self.file_end)
        self.file.write('\n')
        self.file.close()


def quicksort(region_list, attr):
    left = []
    center = []
    right = []

    if len(region_list) <= 1:
        return region_list
    else:
        pivot = getattr(region_list[len(region_list) // 2], attr)
        for i in region_list:
            if getattr(i, attr) > pivot:
                right.append(i)
            elif getattr(i, attr) < pivot:
                left.append(i)
            else:
                center.append(i)
        return quicksort(left, attr) + center + quicksort(right, attr)
