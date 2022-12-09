import collections
import random
# Declare Constants
MAIN_MEMORY_SIZE = 32


# This class holds information about the Page Table Entry
# such as
# 1. process_id: id of process trying to access this page
# 2. page_vpn: virtual page address of the page to be accessed
# 3. page_access: access type viz "W:write" or "R:read"
# 4. page_hit: if page fount in main memory : 1 or page not found: 0
# 5. page_fault: if page not found in the main memory then page fault : 1
#
class PTE:
    def __init__(self, pid, vpn, access):
        self.process_id = pid
        self.page_vpn = vpn
        self.page_access = access
        self.page_hit = 0
        self.page_fault = 0


# This class holds the main page management method
# to add or replace page in the main memory
class PageManagement:
    def __init__(self):
        self.dirty_bits_count = None
        self.page_hit_count = None
        self.page_fault_count = None
        self.disk_access_count = None
        self.index = None

    def AddOrReplacePageInMainMemory(self, main_memory, pte, process_memory_pair):
        # check if this page is already present in main memory for that process
        if (pte.process_id, pte.page_vpn) in process_memory_pair:

            if pte.page_access == "W\n":
                self.dirty_bits_count += 1

            if pte.page_access == "R\n":
                self.disk_access_count += 1

            self.page_hit_count += 1
            return

        # when main memory is full and page fault
        if self.index >= MAIN_MEMORY_SIZE:
            # remove random page from main memory and add one page
            # identify random page using randint() method
            # and remove that page from main_memory
            random_index = random.randint(0, 31)

            # remove the page entry from set of process_memory_pair list too
            process_memory_pair.remove((main_memory[random_index].process_id, main_memory[random_index].page_vpn))

            # replace pte to the main memory
            main_memory[random_index] = pte
            # add new pte to list of visited pages
            process_memory_pair.add((pte.process_id, pte.page_vpn))

            self.page_fault_count += 1
            return

        main_memory.append(pte)
        process_memory_pair.add((pte.process_id, pte.page_vpn))
        self.index += 1
        pte.page_fault += 1
        self.page_fault_count += 1

    def main(self):
        file = open("data1.txt")

        # this is a list holding a set of process_id and virtual page number
        process_memory_pair = set()

        self.page_fault_count = 0
        self.page_hit_count = 0
        self.dirty_bits_count = 0
        self.disk_access_count = 0
        self.index = 0
        print("process_id \\t page.vpn \\t page.page_hit \\t  page.page_fault")

        hashmap = {}
        main_memory = []
        main_memory = collections.deque(main_memory)
        for line in file:
            x = line.split("\t")
            # since first 9 bits are offset 2^9 = 512
            # thus to get virtual page number virtual address >> 9
            # or divide by 512
            pte = PTE(x[0], int(x[1])//512, x[2])
            self.AddOrReplacePageInMainMemory(main_memory, pte, process_memory_pair)

        for pte in main_memory:
            print(str(pte.process_id) + "\t" + str(pte.page_vpn) + "\t" + str(pte.page_hit) + "\t" + str(pte.page_fault))

        print("number of page faults ", self.page_fault_count)
        print("number of page hits ->", self.page_hit_count)
        print("number of dirty bits ->", self.dirty_bits_count)
        print("number of disk access ->", self.disk_access_count)


pm = PageManagement()
pm.main()